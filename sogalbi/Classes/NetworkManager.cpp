#include "pch.h"
#include "..\..\Common\Packet.h"
#include "Ws2tcpip.h"
#include "NetworkManager.h"

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

void NetworkManager::sendPacket(const COMMON::PACKET_ID packetId, const short dataSize, char* pData)
{
	char data[COMMON::MAX_PACKET_SIZE] = { 0, };

	// 헤더
	COMMON::PacketHeader packetHeader{ packetId,dataSize };
	memcpy(data, (char*)&packetHeader, COMMON::PACKET_HEADER_SIZE);

	// 데이터
	if (dataSize > 0)
		memcpy(data + COMMON::PACKET_HEADER_SIZE, pData, dataSize);

	// 전송
	send(_sock, data, dataSize + COMMON::PACKET_HEADER_SIZE, 0);
}

NetworkManager* NetworkManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new NetworkManager();
	return _instance;
}

// 성공시 true, 실패시 false 반환
void NetworkManager::connectTcp(std::string serverIp, int serverPort)
{
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverIp.c_str(), static_cast<void*>(&serverAddr.sin_addr.s_addr));
	serverAddr.sin_port = htons(serverPort);
	
	auto result = connect(_sock,(SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (result == SOCKET_ERROR)
		log("connet() failed");
}

void NetworkManager::sendPacket_LogIn(std::string authToken)
{
	auto packetId = COMMON::PACKET_ID::CHANNEL_JOIN_REQ;
	auto data = COMMON::PacketLoginReq{};
	strncpy_s(data._authToken, authToken.c_str(), strlen(authToken.c_str()));
	sendPacket(packetId, sizeof(data), (char*)&data);
	log("packet sent");
}
