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
	LoadConfig();

	m_IsReady = true;
	m_dbisRunning = true;

	return COMMON::ERROR_CODE::NONE;
}

void App::Run()
{
	std::thread dbThread([&]() {
		StateCheckAndSubmit(); //every 3 seconds, submit serverstatus to DB,
	});

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

	m_dbisRunning = false;
	dbThread.join();
}

void App::StateCheckAndSubmit()
{
	std::chrono::system_clock::time_point before = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point now= std::chrono::system_clock::now();;
	std::chrono::duration<float> dur = std::chrono::duration<float>::zero();;
	
	while (m_dbisRunning)
	{
		before = now;
		now = std::chrono::system_clock::now();
		std::chrono::duration<float> dt = now - before;
		dur += dt;
		if (dur.count() > DBSubmitInterval)
		{
			dur = std::chrono::duration<float>::zero();

			int curUserCount = m_pUserMgr->GetCurrentUserCount();
			m_pDB->SubmitState(m_pServerConfig->MAX_USERCOUNT_PER_CHANNEL, curUserCount);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
	return;
}


COMMON::ERROR_CODE App::LoadConfig()
{
	// [TODO]...

	return COMMON::ERROR_CODE::NONE;
}
