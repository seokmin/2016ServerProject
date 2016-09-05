#include "pch.h"
#include "..\..\Common\Common.h"
#include "Ws2tcpip.h"
#include "NetworkManager.h"
#include "ClientLogger.h"

NetworkManager* NetworkManager::_instance = nullptr;

NetworkManager::NetworkManager()
{
	initTcp();
}

void NetworkManager::initTcp()
{
	// 윈속 초기화
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		log("WSAStartup failed");
		return;
	}

	// socket()
	_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (_sock == INVALID_SOCKET)
	{
		log("socket() failed");
		return;
	}
}

// 패킷 큐에서 딱 하나만 만들어서 리턴한다
void NetworkManager::recvAndGivePacketsToCallbackFuncThread()
{
	using namespace COMMON;

	// 버퍼에 남아있는 데이터의 양. 일관성을 지켜주려면 초기값은 당연히 0
	auto remainingDataSizeInRecvBuffer = 0;

	while (true)
	{
		// 소켓 연결이 끊겼으면 알아서 recv 시도를 그만둔다.
		if (_sock <= 0)
			break;
		// 받을 수 있을 만큼 데이터를 받는다. 이때 채우기 시작하는 위치는 남아있던 데이터의 바로 뒤
		auto receivedSize = recv(_sock, _recvBuffer + remainingDataSizeInRecvBuffer, sizeof(_recvBuffer), 0);
		if (receivedSize == SOCKET_ERROR)
			return;


		// 버퍼에 남아있는 데이터의 양. 일관성을 지켜주려면 recv한 만큼 늘려주는 것이 당연.
		remainingDataSizeInRecvBuffer += receivedSize;
		// 다음번에 만들 패킷 위치는 recv버퍼 맨 앞부터.
		auto nextPacketAddress = _recvBuffer;

		while (true)
		{
			// 헤더를 읽을 수 있을 때
			if (remainingDataSizeInRecvBuffer >= PACKET_HEADER_SIZE)
			{
				// 헤더를 만든다.
				auto packetHeader = (PacketHeader*)nextPacketAddress;
				// 바디를 만들기에도 충분할 때
				if (remainingDataSizeInRecvBuffer >= packetHeader->_bodySize + PACKET_HEADER_SIZE)
				{
					static bool processDone = false;

					// 프로세싱 함수가 처리될 때까지 기다림
					processDone = false;
					auto processLambda = [&]() {
						_callbackFunc(packetHeader->_id, packetHeader->_bodySize, nextPacketAddress + PACKET_HEADER_SIZE);
						processDone = true;
					};

					// 여기까지 왔다면 패킷을 만들 수 있는 것. 패킷을 부탁한 쪽에 넘긴다.
					Director::getInstance()->getScheduler()->performFunctionInCocosThread(processLambda);

					while (processDone == false)
						std::this_thread::sleep_for(std::chrono::milliseconds(0));


					// 다음에 만들 패킷의 주소는 지금 만든 패킷의 바로 뒤이다.
					nextPacketAddress += PACKET_HEADER_SIZE + packetHeader->_bodySize;
					// 또한, 하나 만들었으니 남은 데이터 양이 줄어들었다는 것을 표시한다.
					remainingDataSizeInRecvBuffer -= PACKET_HEADER_SIZE + packetHeader->_bodySize;
					continue; // 하나 만들었으니 다음 패킷을 만들러 출발한다.
				}
			}
			// 여기 왔다는 것은 패킷을 만들기에 데이터가 모자라다는 것.
			// 또한 기존 만들었던 패킷들을 callbackFunc()가 전부 소화했다는 것.
			// 남은 재료를 recv버퍼의 가장 앞으로 당기고 다음번 Recv를 기다린다.
			memcpy(_recvBuffer, nextPacketAddress, remainingDataSizeInRecvBuffer);
			break;
		}
		// 만들 수 있을 만큼 다 만들었으므로 다음번 Recv를 기다린다.
	}
}



void NetworkManager::setRecvCallback(std::function<void(const COMMON::PACKET_ID, const short, char*)> callbackFunc)
{
	_callbackFunc = callbackFunc;
}

void NetworkManager::disconnectTcp()
{
	closesocket(_sock);
}

// send() 성공여부 반환
bool NetworkManager::sendPacket(const COMMON::PACKET_ID packetId, const short dataSize, char* pData)
{
	_mutex.lock();

	char data[COMMON::MAX_PACKET_SIZE] = { 0, };

	// 헤더
	COMMON::PacketHeader packetHeader{ packetId,dataSize };
	memcpy(data, (char*)&packetHeader, COMMON::PACKET_HEADER_SIZE);

	// 데이터
	if (dataSize > 0)
		memcpy(data + COMMON::PACKET_HEADER_SIZE, pData, dataSize);

	// 전송
	auto result = send(_sock, data, dataSize + COMMON::PACKET_HEADER_SIZE, 0);
	if (result == SOCKET_ERROR)
	{
		ClientLogger::logThreadSafe("send() failed. packet id : " + packetId);
		// disconnect() 해줘야할듯
		_mutex.unlock();
		return false;
	}

	ClientLogger::logThreadSafe("packet send success. packet id : " + packetId);
	_mutex.unlock();
	return true;
}

NetworkManager* NetworkManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new NetworkManager();
	return _instance;
}

// 성공시 true, 실패시 false 반환
bool NetworkManager::connectTcp(std::string serverIp, int serverPort)
{
	auto returnVal = true;
	if (_mutex.try_lock() == false)
		return false;
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverIp.c_str(), static_cast<void*>(&serverAddr.sin_addr.s_addr));
	serverAddr.sin_port = htons(serverPort);

	auto result = connect(_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (result == SOCKET_ERROR)
	{
		ClientLogger::msgBox(L"해당 채널이 응답하지 않습니다.");
		ClientLogger::logThreadSafe("connect() failed");
		returnVal = false;
	}
	// TODO : 짜는 중
	auto newThread = std::thread(std::bind(&NetworkManager::recvAndGivePacketsToCallbackFuncThread, this));
	newThread.detach();
	_mutex.unlock();
	return returnVal;
}