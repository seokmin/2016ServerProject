#include "stdafx.h"

#include "IOCPManager.h"
#include "App.h"

#include <locale>

COMMON::ERROR_CODE App::Init()
{
	std::wcin.imbue(std::locale("korean"));
	m_pServerConfig = std::make_unique<ServerConfig>();
	
	
	wprintf(L"Please Type Server name : ");
	std::wcin >> m_pServerConfig->SERVERNAME;
	wprintf(L"Please Type Server IP : ");
	std::wcin >> m_pServerConfig->IP;
	wprintf(L"Please Type Server Port : ");
	scanf("%u", &m_pServerConfig->Port);
	wprintf(L"Please Type Server Min Bet : ");
	scanf("%u", &m_pServerConfig->minBet);
	wprintf(L"Please Type Server Max Bet : ");
	scanf("%u", &m_pServerConfig->maxBet);

	getchar();

	//network init
	m_pSendPacketQue = std::make_unique<PacketQueue>();
	m_pRecvPacketQue = std::make_unique<PacketQueue>();
	IOCPManager::GetInstance()->InitServer(m_pRecvPacketQue.get(), m_pSendPacketQue.get(), m_pServerConfig.get());

	m_pDB = std::make_unique<DBmanager>();
	m_pDBProc = std::make_unique<DBProcess>();
	m_pUserMgr = std::make_unique<UserManager>();
	m_pRoomMgr = std::make_unique<RoomManager>();

	m_pDB->Init(m_numberOfDBThread);
	m_pDBProc->Init(m_pUserMgr.get(), m_pRoomMgr.get(), m_pSendPacketQue.get(), m_pDB.get());
	m_pUserMgr->Init(m_pRoomMgr.get(), m_pDB.get());
	m_pRoomMgr->Init(m_pUserMgr.get(), m_pDB.get(), m_pSendPacketQue.get(), m_pServerConfig.get());

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
			m_pRoomMgr->RunPostTimeAction();

			auto& packet = m_pRecvPacketQue.get()->ReadFront();
			if (packet.PacketId != 0)
			{
				m_pPacketProc->Process(packet);
				m_pRecvPacketQue.get()->PopFront();
			}

			if (!m_pDB->DBResultEmpty())
			{
				auto& dbRslt = m_pDB->FrontDBResult();
				if (dbRslt._type == JOB_TYPE::SUBMIT_STATE)
				{
					if (dbRslt._type == JOB_TYPE::NONE)
						break;

					m_pDBProc->Process(dbRslt);
					m_pDB->PopDBResult();
				}
				else
				{
					m_pDBProc->Process(dbRslt);
					m_pDB->PopDBResult();
				}
				
			}

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
	std::chrono::system_clock::time_point now= std::chrono::system_clock::now();
	std::chrono::duration<float> dur = std::chrono::duration<float>::zero();
	
	while (m_dbisRunning && m_IsReady)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		dur = std::chrono::duration<float>::zero();

		//int curUserCount = m_pUserMgr->`(); //[TODO]
		int curUserCount = 1;
		m_pDB->SubmitState(m_pServerConfig->MAX_USERCOUNT_PER_CHANNEL, curUserCount, m_pServerConfig.get());		
	}
	return;
}


COMMON::ERROR_CODE App::LoadConfig()
{
	// [TODO]... Serverconfig->LoadConfig()


	return COMMON::ERROR_CODE::NONE;
}
