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

void ClientLogger::msgBox(const wchar_t* message, const wchar_t* title /*= L"JackBlack"*/, bool threaded /*= false*/)
{
	logThreadSafe("MsgBox popup");
	if (threaded)
	{
		auto newThread = std::thread(MessageBoxW, nullptr, message, title, MB_OK);
		newThread.detach();
	}
	else
		MessageBoxW(nullptr, message, title, MB_OK);
}
