#include "stdafx.h"
#include "App.h"
#include "DBmanager.h"

ERROR_CODE App::Init()
{
	// [TODO] ...

	return ERROR_CODE::NONE;
}

void App::Run()
{
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
		
		StateCheckAndSubmit();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
}

void App::StateCheckAndSubmit()
{
	int curUserCount = m_pUserMgr->GetCurrentUserCount();
	m_pDB->SubmitState(m_pServerConfig->MAX_USERCOUNT_PER_CHANNEL, curUserCount);
	return;
}


ERROR_CODE App::LoadConfig()
{
	// [TODO]...

	return ERROR_CODE::NONE;
}
