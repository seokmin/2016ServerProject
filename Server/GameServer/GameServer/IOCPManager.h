#pragma once

struct ServerSetting
{
	int			_portNum;

};

class IOCPManager
{
public:
	IOCPManager*		GetInstance();
	void				InitServer(ServerSetting& setting);
	void				StartServer();
private:
	IOCPManager() {};

	void				WorkerThreadFunc();
	void				ListenThreadFunc();

	HANDLE				CreateIOCP();
	void				BindSocketToIOCP();
public:
private:
	static IOCPManager*	_instance ;
	bool				_initialized = false;

	HANDLE				_completionPort;
	SOCKET				_serverSocket;
};