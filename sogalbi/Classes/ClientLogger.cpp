#include "pch.h"
#include "ClientLogger.h"

void ClientLogger::logThreadSafe(const char* message)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread(
		[message]()
	{
		log(message);
	}
	);
}

void ClientLogger::msgBox(const wchar_t* message, bool threaded /*= false*/)
{
	if (threaded)
	{
		auto newThread = std::thread(MessageBoxW, nullptr, message, L"logger", MB_OK);
		newThread.detach();
	}
	else
		MessageBoxW(nullptr, message, L"logger", MB_OK);
}
