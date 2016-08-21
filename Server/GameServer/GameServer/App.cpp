#include "stdafx.h"

#include "IOCPManager.h"
#include "App.h"

COMMON::ERROR_CODE App::Init()
{
	m_pServerConfig = std::make_unique<ServerConfig>();
	m_pDB = std::make_unique<DBmanager>();
	m_pDB->Init();
	
	m_pUserMgr = std::make_unique<UserManager>();
	m_pRoomMgr = std::make_unique<RoomManager>();
	m_pRoomMgr->Init();
	
	//network init
	m_pSendPacketQue = std::make_unique<PacketQueue>();
	m_pRecvPacketQue = std::make_unique<PacketQueue>();
	IOCPManager::GetInstance()->InitServer(m_pRecvPacketQue.get(), m_pSendPacketQue.get(), m_pServerConfig.get());

	m_pPacketProc = std::make_unique<PacketProcess>();
	m_pPacketProc->Init(m_pUserMgr.get(), m_pRoomMgr.get(), m_pRecvPacketQue.get(), m_pSendPacketQue.get());

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

	IOCPManager::GetInstance()->StartServer();

	while (m_IsReady)
	{

		while (true)
		{
			auto& packet = m_pRecvPacketQue.get()->ReadFront();
			if (packet.PacketId == 0)
				break;

			m_pPacketProc->Process(packet);
			m_pRecvPacketQue.get()->PopFront();
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}

	m_dbisRunning = false;
	dbThread.join();
}

void App::Release()
{
	//delete single tone member
	IOCPManager::DelInstance();
	
	//logger delete
	Logger::DelInstance();
}

void App::StateCheckAndSubmit()
{
	std::chrono::system_clock::time_point before = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point now= std::chrono::system_clock::now();;
	std::chrono::duration<float> dur = std::chrono::duration<float>::zero();;
	
	while (m_dbisRunning && m_IsReady)
	{
		before = now;
		now = std::chrono::system_clock::now();
		std::chrono::duration<float> dt = now - before;
		dur += dt;
		if (dur.count() > DBSubmitInterval)
		{
			dur = std::chrono::duration<float>::zero();

			//int curUserCount = m_pUserMgr->GetCurrentUserCount(); //[TODO]
			int curUserCount = 1;
			ERROR_CODE result;
			do {
				 result = m_pDB->SubmitState(m_pServerConfig->MAX_USERCOUNT_PER_CHANNEL, curUserCount, m_pServerConfig.get());
			} while (result != ERROR_CODE::NONE);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
	return;
}


COMMON::ERROR_CODE App::LoadConfig()
{
	// [TODO]... Serverconfig->LoadConfig()


	return COMMON::ERROR_CODE::NONE;
}
