#pragma once

class NetworkManager
{
public:
	static NetworkManager* getInstance();

	void connectTcp(std::string serverIp, int serverPort);

	void sendPacket_LogIn(std::string authToken);


private:
	NetworkManager();
	void	initTcp();
	void	sendPacket(const COMMON::PACKET_ID packetId, const short dataSize, char* pData);

private:
	static NetworkManager* _instance;
	std::mutex	_mutex;
	SOCKET _sock;
	std::string	_serverIp;
	int			_port;
};