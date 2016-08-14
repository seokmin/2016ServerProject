#pragma once
#include <memory>

#include "Common.h"
#include "INetworkManager.h"
//#include "UserManager.h" //[TODO]
//#include "RoomManager.h" //[TODO]
#include "PacketProcess.h"
// #include "ILog.h" //[TODO]

class App
{
public:
	App() {};
	~App() { Release(); };

	ERROR_CODE Init();
	void Run();
	void Stop() { m_IsReady = false; };

	ERROR_CODE LoadConfig();

private:
	void Release() {};

private:
	bool m_IsReady = false;
	//[TODO]std::unique_ptr<ILog> m_pLogger;

	std::unique_ptr<NetworkConfig> m_pServerConfig;
	std::unique_ptr<INetworkManager> m_pNetwork;	
	std::unique_ptr<PacketProcess> m_pPacketProc;
	//[TODO] std::unique_ptr<UserManager> m_pUserMgr;
	//[TODO] std::unique_ptr<RoomManager> m_pRoomMgr;
};