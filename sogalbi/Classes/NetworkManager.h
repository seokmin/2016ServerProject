#pragma once
class NetworkManager
{
public:
	static NetworkManager* getInstance();

	void connectTcp(std::string serverIp, int serverPort);

	void sendPacket_LogIn();
private:
	NetworkManager();
	void initTcp();

private:
	static NetworkManager* _instance;
	SOCKET _sock;
	std::string	_serverIp;
	int			_port;
};