#include "stdafx.h"
#include "NetworkSetting.h"
#include "BufferQueue.h"
#include <vector>

#include "IOCPManager.h"


IOCPManager* IOCPManager::_instance = nullptr;

// 채널 설정을 Json에서 읽어온다
// TODO : 더미 데이터 넣어뒀음. json에서 읽어오게 제대로 고쳐야 함
void IOCPManager::LoadChannelSettingFromJson()
{
	auto setting = NetworkSetting{};
	setting._maxBufferCount = 10000;
	setting._maxBufferSize = 1024 * 4; // byte
	setting._maxSessionCount = 5000;
	setting._portNum = 34343;
	
	_setting = setting;
}

void IOCPManager::WorkerThreadFunc()
{
	DWORD transferedByte = 0;
	unsigned sessionIndex = 0u;
	IOInfo ioInfo;

	while (true)
	{
		GetQueuedCompletionStatus(_completionPort, &transferedByte,
			(PULONG_PTR)&sessionIndex, (LPOVERLAPPED*)&ioInfo, INFINITE);
		
		auto sock = _sessionPool[sessionIndex]._socket;

		if (ioInfo._rwMode == IOInfo::RWMode::READ)
		{

		}
		else
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
		_sessionPoolMutex.unlock();
		auto& newSession = _sessionPool.at(newSessionIndex);
		ZeroMemory(&newSession, sizeof(newSession));
		// 세션에 새로운 클라 정보 기입
		newSession._sockAddr = clientAddr;
		newSession._socket = newSocket;


		auto ioInfo = new IOInfo();
		// 해당 io에 사용할 버퍼를 버퍼 풀에서 하나 얻어온다.
		ioInfo->_wsaBuf.buf = BufferQueue::GetInstance()->GetBufferThreadSafe();
		ioInfo->_wsaBuf.len = _setting._maxBufferSize;
		ioInfo->_rwMode = IOInfo::RWMode::READ;
		ZeroMemory(&ioInfo->_overlapped, sizeof(OVERLAPPED));

		// Completion Port에 새로운 세션을 등록함
		BindSessionToIOCP(newSession);

		DWORD recvSize = 0, flags = 0;

		// Recv 걸어놓는다. 완료되면 worker thread로 넘어감
		WSARecv(newSession._socket,	// 소켓
			&ioInfo->_wsaBuf,		// 해당 recv에 사용할 버퍼
			1,						// 사용할 버퍼 개수
			&recvSize,&flags,&ioInfo->_overlapped,nullptr);
	}
}

void IOCPManager::StartServer()
{
	// 채널 셋팅 읽어오는 부분
	LoadChannelSettingFromJson();


	// IOCP를 만든다.
	_completionPort = CreateIOCP();

	// 세션 풀을 만든다.
	CreateSessionPool();

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
	// 서버 소켓 활성화
	// TODO : backlog 적정값 결정 필요
	listen(_serverSocket, 5);

	// listen 스레드를 만든다.
	auto listenThread = std::thread(std::bind(&IOCPManager::ListenThreadFunc, this));
	listenThread.detach();


	// worker 스레드 풀을 만든다.
	auto sysInfo = SYSTEM_INFO{};
	GetSystemInfo(&sysInfo); // 시스템 정보 알아오기. 코어 개수 파악을 위함
	auto numThread = sysInfo.dwNumberOfProcessors * 2;
	// 코어 개수 * 2개만큼 worker 스레드 풀을 만든다.
	for (auto i = 0u; i < numThread; ++i)
	{
		// 각 클라이언트에 붙을 worker 스레드
		auto workerThread = std::thread(std::bind(&IOCPManager::WorkerThreadFunc,this));
		workerThread.detach();
	}
}

HANDLE IOCPManager::CreateIOCP()
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
}

void IOCPManager::CreateSessionPool()
{
	// 방어코드
	if (_sessionPool.size() != 0)
		return;

	for (unsigned i = 0; i < _setting._maxSessionCount; ++i)
	{
		_sessionPool.emplace_back(std::move(SessionInfo{}));
		_sessionIndexPool.push_back(i);
	}
}

void IOCPManager::BindSessionToIOCP(SessionInfo& targetSession)
{
	CreateIoCompletionPort((HANDLE)targetSession._socket, _completionPort, (ULONG_PTR)&targetSession._index, 0);
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
