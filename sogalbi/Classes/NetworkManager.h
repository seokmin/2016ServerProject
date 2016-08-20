#pragma once

class NetworkManager
{
public:
	static NetworkManager* getInstance();

	bool connectTcp(std::string serverIp, int serverPort);

	bool sendPacket(const COMMON::PACKET_ID packetId, const short dataSize, char* pData);

private:
	NetworkManager();
	void	initTcp();

private:
	static NetworkManager* _instance;
	std::mutex	_mutex;
	SOCKET _sock;
	std::string	_serverIp;
	int			_port;
};