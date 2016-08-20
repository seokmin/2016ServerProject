#pragma once

#include "INetworkManager.h"
#include "DBmanager.h"
#include "PacketProcess.h"
#include "UserManager.h"
#include "RoomManager.h"

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
	void Release() {};
	void StateCheckAndSubmit();

private:
	bool m_IsReady = false;
	//std::unique_ptr<ILog> m_pLogger;

	std::unique_ptr<NetworkConfig> m_pServerConfig;
	std::unique_ptr<INetworkManager> m_pNetwork;	
	std::unique_ptr<DBmanager> m_pDB;
	std::unique_ptr<PacketProcess> m_pPacketProc;
	std::unique_ptr<UserManager> m_pUserMgr;
	std::unique_ptr<RoomManager> m_pRoomMgr;
};