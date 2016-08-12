#pragma once
class NetworkManager
{
public:
	static NetworkManager* getInstance();

	void init();
	void mainLoop();
	bool connectTcp(std::string serverIp, int serverPort);
private:
	NetworkManager();
	void initTcp();

private:
	static NetworkManager* _instance;
	SOCKET _sock;
};