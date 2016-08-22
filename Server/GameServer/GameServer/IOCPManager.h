#pragma once
#include <deque>
#include <vector>
#include <mutex>
#include "NetworkSetting.h"

class PacketQueue;
class ServerConfig;

struct SessionInfo
{
	unsigned			_index = 0;
	SOCKET				_socket = -1;
	sockaddr_in			_sockAddr;
	char*				_recvBuffer = nullptr;
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
	unsigned			_sessionIndex;
};

class IOCPManager
{
public:
	static IOCPManager*		GetInstance();
	static void				DelInstance();

	COMMON::ERROR_CODE		InitServer(PacketQueue* recvPacketQueue, PacketQueue* sendPacketQueue, ServerConfig* serverConfig);
	COMMON::ERROR_CODE		StartServer();
private:
	IOCPManager() {};

	void					LoadChannelSettingFromServerConfig(ServerConfig* serverconfig);
	HANDLE					CreateIOCP();
	void					CreateSessionPool();
	void					ReleaseSessionPool();

	void					WorkerThreadFunc();
	void					ListenThreadFunc();
	void					SendThreadFunc();

	void					BindSessionToIOCP(SessionInfo* targetSession);

public:
private:
	static IOCPManager*		_instance ;
	bool					_initialized = false;

	NetworkSetting			_setting;

	HANDLE					_completionPort;
	SOCKET					_serverSocket;

	std::vector<SessionInfo*>	_sessionPool;
	std::deque<int>				_sessionIndexPool;
	std::mutex					_sessionPoolMutex;
	PacketQueue*				_recvPacketQueue = nullptr;
	PacketQueue*				_sendPacketQueue = nullptr;
};