#include "stdafx.h"
#include "App.h"

COMMON::ERROR_CODE App::Init()
{
	m_pServerConfig = std::make_unique<NetworkConfig>();
	m_pNetwork = std::make_unique<INetworkManager>();
	m_pDB = std::make_unique<DBmanager>();
	m_pPacketProc = std::make_unique<PacketProcess>();
	m_pUserMgr = std::make_unique<UserManager>();
	m_pRoomMgr = std::make_unique<RoomManager>();

	return COMMON::ERROR_CODE::NONE;
}

void App::Run()
{
	StateCheckAndSubmit(); //every 3 seconds, submit serverstatus to DB,
	/*std::thread logicThread([&]() {
	
	});
*/
	while (m_IsReady)
	{
		m_pNetwork->Run();

		while (true)
		{
			auto packetInfo = m_pNetwork->GetPacketInfo();

			if (packetInfo.PacketId == 0)
			{
				break;
			}

			m_pPacketProc->Process(packetInfo);
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
}

void App::StateCheckAndSubmit()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	int curUserCount = m_pUserMgr->GetCurrentUserCount();
	m_pDB->SubmitState(m_pServerConfig->MAX_USERCOUNT_PER_CHANNEL, curUserCount);
	return;
}


COMMON::ERROR_CODE App::LoadConfig()
{
	// [TODO]...

	return COMMON::ERROR_CODE::NONE;
}
