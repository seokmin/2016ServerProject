
#include <thread>
#include <chrono>
#include <iostream>

#include "App.h"

ERROR_CODE App::Init()
{
	// ...

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

		m_pPacketProc->StateCheck();

		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
}

ERROR_CODE App::LoadConfig()
{
	// ...

	return ERROR_CODE::NONE;
}
