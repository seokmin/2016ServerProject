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

COMMON::RecvPacketInfo NetworkManager::createPacketFromRecvBuffer()
{
	auto receivedSize = recv(_sock, _recvBuffer, sizeof(_recvBuffer), 0);
	auto remainingDataSizeInBuf = receivedSize;
	
	// 헤더를 읽을 수 있을 때
	while (remainingDataSizeInBuf >= COMMON::PACKET_HEADER_SIZE)
	{

	}
	return COMMON::RecvPacketInfo{};
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

void NetworkManager::setRecvCallback(std::function<void(const COMMON::PACKET_ID, const short, char*)> callbackFunc)
{
	// TODO : 짜는 중
	auto recvThread = [&]()
	{
		createPacketFromRecvBuffer();
	};
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
	_mutex.lock();
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
	_mutex.unlock();
	return returnVal;
}