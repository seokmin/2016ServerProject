#pragma once
#include <deque>
#include <vector>
#include <mutex>
#include "NetworkSetting.h"

class PacketQueue;

struct SessionInfo
{
	unsigned			_index;
	SOCKET				_socket;
	sockaddr_in			_sockAddr;
	bool				IsConnected() { return _socket > 0 ? true : false; }
};

struct IOInfo
{
	OVERLAPPED			_overlapped;
	WSABUF				_wsaBuf; // WSARecv¿ë
	enum class RWMode : short
	{
		READ = 0,
		WRITE = 1
 	} _rwMode;
};

class IOCPManager
{
public:
	static IOCPManager*		GetInstance();
	static void				DelInstance();
	void					StartServer();
private:
	IOCPManager() {};

	void					LoadChannelSettingFromJson();
	HANDLE					CreateIOCP();
	void					CreateSessionPool();

	void					WorkerThreadFunc();
	void					ListenThreadFunc();

	void					BindSessionToIOCP(SessionInfo& targetSession);


public:
private:
	static IOCPManager*		_instance ;
	bool					_initialized = false;

	NetworkSetting			_setting;

	HANDLE					_completionPort;
	SOCKET					_serverSocket;

	std::vector<SessionInfo>	_sessionPool;
	std::deque<int>				_sessionIndexPool;
	std::mutex					_sessionPoolMutex;
	PacketQueue*				_recvPacketQueue;
};