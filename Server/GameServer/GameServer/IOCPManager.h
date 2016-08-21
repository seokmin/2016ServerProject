#pragma once
#include <deque>
#include <vector>
#include <mutex>
#include "NetworkSetting.h"

class PacketQueue;
class ServerConfig;

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
	WSABUF				_wsaBuf; // WSARecv��
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

	void					InitServer(PacketQueue* recvPacketQueue, PacketQueue* sendPacketQueue, ServerConfig* serverConfig);
	COMMON::ERROR_CODE		StartServer();
private:
	IOCPManager() {};

	void LoadChannelSettingFromServerConfig(ServerConfig* serverconfig);
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
	PacketQueue*				_recvPacketQueue = nullptr;
	PacketQueue*				_sendPacketQueue = nullptr;
};