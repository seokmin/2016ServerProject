#pragma once

#include "DBmanager.h"
#include "PacketProcess.h"
#include "UserManager.h"
#include "RoomManager.h"
#include "ServerConfig.h"
#include "Logger.h"
#include "PacketQueue.h"
#include "DBProcess.h"

class App
{
public:
	App() {};
	~App() { Release(); };

	COMMON::ERROR_CODE Init();
	void Run();
	void Stop() { m_IsReady = false; };

	COMMON::ERROR_CODE LoadConfig();

private:
	void Release();
	void StateCheckAndSubmit();

private:
	bool m_IsReady = false;
	bool m_dbisRunning = false;
	const int DBSubmitInterval = 3;
	const short m_numberOfDBThread = ServerConfig::numberOfDBThread;

	std::unique_ptr<ServerConfig> m_pServerConfig;
	std::unique_ptr<DBmanager> m_pDB;
	std::unique_ptr<DBProcess> m_pDBProc;
	std::unique_ptr<PacketProcess> m_pPacketProc;
	std::unique_ptr<UserManager> m_pUserMgr;
	std::unique_ptr<RoomManager> m_pRoomMgr;
	std::unique_ptr<PacketQueue> m_pRecvPacketQue;
	std::unique_ptr<PacketQueue> m_pSendPacketQue;
};