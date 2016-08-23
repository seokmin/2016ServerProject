#include "stdafx.h"
#include <vector>

#include "ServerConfig.h"
#include "NetworkSetting.h"
#include "BufferQueue.h"
#include "PacketQueue.h"

#include "IOCPManager.h"


IOCPManager* IOCPManager::_instance = nullptr;

void IOCPManager::LoadChannelSettingFromServerConfig(ServerConfig* serverconfig)
{
	auto setting = NetworkSetting{};
	setting._maxSessionCount = serverconfig->MAX_USERCOUNT_PER_CHANNEL + serverconfig->ExtraClientCount;
	setting._portNum = serverconfig->Port;
	setting._backLog = serverconfig->BackLogCount;
	setting._maxSessionRecvBufferSize = serverconfig->MAX_SESSION_RECV_BUFFER_SIZE;

	_setting = setting;
}

void IOCPManager::WorkerThreadFunc()
{
	DWORD transferedByte = 0;
	int *a = nullptr;
	IOInfo* ioInfo;

	while (true)
	{
		GetQueuedCompletionStatus(_completionPort, &transferedByte,
			(PULONG_PTR)&a, (LPOVERLAPPED*)&ioInfo, INFINITE);

		auto sessionIndex = ioInfo->_sessionIndex;
		SessionInfo* pSession = _sessionPool.at(sessionIndex);

		if (ioInfo->_rwMode == IOInfo::RWMode::READ)
		{
			auto headerPos = pSession->_recvBuffer;
			auto receivePos = ioInfo->_wsaBuf.buf;
			// 아직 처리 안된 데이터 양 = 끝지점 - 시작지점
			auto totalDataSizeInBuf = receivePos + transferedByte - pSession->_recvBuffer;
			// 패킷으로 만들어지길 기다리는 데이터의 사이즈
			auto remainingDataSizeInBuf = totalDataSizeInBuf;

			// 패킷 조제 루프
			while (remainingDataSizeInBuf >= PACKET_HEADER_SIZE)
			{ // 헤더를 들여다 보기에 충분한 데이터가 있다면
				// 헤더를 들여다본다
				auto header = (PacketHeader*)headerPos;
				auto bodySize = header->_bodySize;
				// 패킷 하나를 온전히 만들 수 있을 때
				if (PACKET_HEADER_SIZE + bodySize >= remainingDataSizeInBuf)
				{
					// 패킷을 만든다.
					auto newPacketInfo = RecvPacketInfo{};
					newPacketInfo.PacketId = header->_id;
					newPacketInfo.PacketBodySize = bodySize;
					newPacketInfo.pRefData = headerPos + PACKET_HEADER_SIZE; // 바디 위치
					newPacketInfo.SessionIndex = sessionIndex;

					// 패킷큐에 집어넣는다. 복사로 동작하므로 문제없다.
					_recvPacketQueue->PushBack(newPacketInfo);

					// 패킷을 하나 온전히 만들었으므로 다음번 헤더 자리를 지정하고, 남은 데이터 사이즈를 갱신해준다.
					headerPos += PACKET_HEADER_SIZE + bodySize;
					remainingDataSizeInBuf -= PACKET_HEADER_SIZE + bodySize;
				}
				else // 헤더는 있는데 데이터가 모자라서 패킷 하나를 통짜로 만들 수 없을 때
					break;
			}
			// 만들 수 있는 놈들은 다 패킷으로 만들었다. 남은 찌꺼기들을 버퍼의 맨 앞으로 당겨주자.
			// 남은 찌꺼기들이라고 하더라도 시작부분은 무조건 정수리일 것이다.
			memcpy_s(pSession->_recvBuffer, _setting._maxSessionRecvBufferSize, headerPos, remainingDataSizeInBuf);

			// 이제 만들 수 있는 패킷은 다 만들었다. Recv 걸어주자.
			ZeroMemory(&ioInfo->_overlapped, sizeof(OVERLAPPED));
			ioInfo->_wsaBuf.buf = pSession->_recvBuffer + remainingDataSizeInBuf;// 아직 미완 패킷이 있을 수 있으므로 recv는 미완패킷 뒤에다가 받는다.
			ioInfo->_wsaBuf.len = _setting._maxSessionRecvBufferSize - remainingDataSizeInBuf;
			ioInfo->_rwMode = IOInfo::RWMode::READ;
			DWORD recvSize = 0, flags = 0;
			WSARecv(pSession->_socket,	// 소켓
				&ioInfo->_wsaBuf,		// 해당 recv에 사용할 버퍼
				1,						// 사용할 버퍼 개수
				&recvSize, &flags, &ioInfo->_overlapped, nullptr);
		}
		else // RWMode::WRITE
		{

		}
	}
}

void IOCPManager::ListenThreadFunc()
{
	while (true)
	{

		auto clientAddr = sockaddr_in{};
		int addrLen = sizeof(clientAddr);
		// 여기서 block
		auto newSocket = accept(_serverSocket, (SOCKADDR*)&clientAddr, &addrLen);


		// 동접자 초과..
		while (_sessionIndexPool.empty())
			Sleep(1000); // TODO : 동접자 초과시 처리. 일단은 자리 빌때까지 기다림

		// 새로 접속한 유저가 사용할 세션을 노는 세션중에 얻어온다.
		_sessionPoolMutex.lock();
		auto newSessionIndex = _sessionIndexPool.front();
		_sessionIndexPool.pop_front();
		_sessionPoolMutex.unlock();
		auto newSession = _sessionPool.at(newSessionIndex);
		// 세션에 새로운 클라 정보 기입
		newSession->_sockAddr = clientAddr;
		newSession->_socket = newSocket;


		auto ioInfo = new IOInfo();
		ZeroMemory(&ioInfo->_overlapped, sizeof(OVERLAPPED));
		ioInfo->_wsaBuf.buf = newSession->_recvBuffer;
		ioInfo->_wsaBuf.len = _setting._maxSessionRecvBufferSize;
		ioInfo->_rwMode = IOInfo::RWMode::READ;
		ioInfo->_sessionIndex = newSessionIndex;

		// Completion Port에 새로운 세션을 등록함
		BindSessionToIOCP(newSession);

		DWORD recvSize = 0, flags = 0;

		// Recv 걸어놓는다. 완료되면 worker thread로 넘어감
		WSARecv(newSession->_socket,	// 소켓
			&ioInfo->_wsaBuf,		// 해당 recv에 사용할 버퍼
			1,						// 사용할 버퍼 개수
			&recvSize, &flags, &ioInfo->_overlapped, nullptr);
	}
}

// TODO : 비효율적으로 계속 루프를 돌고 있는데, 패킷 큐에 데이터가 들어올 때 WsaSend를 호출하게 바꿀 것임
void IOCPManager::SendThreadFunc()
{
	while (true)
	{
		if (_sendPacketQueue->IsEmpty())
		{
			// 우선 작업이 있는 다른 스레드가 있으면 양보한다
			std::this_thread::sleep_for(std::chrono::milliseconds(0));
			continue;
		}
		// send를 해도 되는 상황인지 확인해서 안되는 상황이면 다음으로 미룬다.
		auto packetToSend = _sendPacketQueue->ReadFront();
		auto targetSession = _sessionPool.at(packetToSend.SessionIndex);
		auto headerToSend = PacketHeader{(PACKET_ID)packetToSend.PacketId,packetToSend.PacketBodySize};
		send(targetSession->_socket, (char*)&headerToSend, PACKET_HEADER_SIZE, 0);
		send(targetSession->_socket, packetToSend.pRefData, packetToSend.PacketBodySize, 0);
		_sendPacketQueue->PopFront();
	}
}

COMMON::ERROR_CODE IOCPManager::StartServer()
{
	if (_initialized == false)
		return ERROR_CODE::IOCP_START_FAIL_NOT_INITIALIZED;

	// 서버 소켓 활성화
	listen(_serverSocket, _setting._backLog);

	// listen 스레드 돌린다.
	auto listenThread = std::thread(std::bind(&IOCPManager::ListenThreadFunc, this));
	listenThread.detach();

	// worker 스레드 돌린다.
	auto sysInfo = SYSTEM_INFO{};
	GetSystemInfo(&sysInfo); // 시스템 정보 알아오기. 코어 개수 파악을 위함
	auto numThread = sysInfo.dwNumberOfProcessors * 2;
	// 코어 개수 * 2개만큼 worker 스레드 풀을 만든다.
	for (auto i = 0u; i < numThread; ++i)
	{
		// 각 클라이언트에 붙을 worker 스레드
		auto workerThread = std::thread(std::bind(&IOCPManager::WorkerThreadFunc, this));
		workerThread.detach();
	}

	// send 스레드 돌린다.
	auto sendThread = std::thread(std::bind(&IOCPManager::SendThreadFunc, this));
	sendThread.detach();

	return ERROR_CODE::NONE;
}

HANDLE IOCPManager::CreateIOCP()
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
}

void IOCPManager::CreateSessionPool()
{
	// 방어코드
	if (_sessionPool.empty() == false)
		return;

	for (unsigned i = 0; i < _setting._maxSessionCount; ++i)
	{
		auto newSession = new SessionInfo();
		newSession->_index = i;
		// 세션마다 가지고 있는 버퍼를 새로 만든다
		newSession->_recvBuffer = new char[_setting._maxSessionRecvBufferSize];
		_sessionPool.push_back(newSession);
		_sessionIndexPool.push_back(i);
	}
}

void IOCPManager::ReleaseSessionPool()
{
	for (auto& i : _sessionPool)
	{
		delete[] i->_recvBuffer;
		// TODO :: Session pool 릴리즈
	}
}

void IOCPManager::BindSessionToIOCP(SessionInfo* targetSession)
{
	CreateIoCompletionPort((HANDLE)targetSession->_socket, _completionPort, (ULONG_PTR)nullptr, 0);
}

IOCPManager* IOCPManager::GetInstance()
{
	if (_instance == nullptr)
		_instance = new IOCPManager();
	return _instance;
}

void IOCPManager::DelInstance()
{
	if (_instance != nullptr)
	{
		delete _instance;
		_instance = nullptr;
	}
	return;
}

COMMON::ERROR_CODE IOCPManager::InitServer(PacketQueue* recvPacketQueue, PacketQueue* sendPacketQueue, ServerConfig* serverConfig)
{
	if (recvPacketQueue == nullptr || sendPacketQueue == nullptr)
		return ERROR_CODE::IOCP_INIT_FAIL_DONOT_GIVEME_NULLPTR;

	// 서버설정에서 채널설정을 불러온다
	LoadChannelSettingFromServerConfig(serverConfig);
	_recvPacketQueue = recvPacketQueue;
	_sendPacketQueue = sendPacketQueue;

	// 세션 풀을 만든다.
	CreateSessionPool();

	// IOCP를 만든다.
	_completionPort = CreateIOCP();

	// 윈속 초기화
	auto wsaData = WSADATA{};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		Beep(1000, 1000); // TODO : 에러 발생시 일단 beep 해둠. 나중에 에러처리 필요

	// 서버 소켓 셋팅
	// TODO : SOCK_STREAM, WSA_FLAG_OVERLAPPED의 의미는 잘 모르겠다. 주석 바람
	_serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
	sockaddr_in socketAddress;
	ZeroMemory(&socketAddress, sizeof(socketAddress));
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddress.sin_port = htons(_setting._portNum);

	bind(_serverSocket, (sockaddr*)&socketAddress, sizeof(socketAddress));

	_initialized = true;
	return ERROR_CODE::NONE;
}