#include "stdafx.h"
#include "Logger.h"
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string

Logger* Logger::g_instance = nullptr;


Logger * Logger::GetInstance()
{
	if (g_instance == nullptr)
	{
		g_instance = new Logger();
		return g_instance;
	}
	return g_instance;
}

void Logger::DelInstance()
{
	if (g_instance != nullptr)
	{
		delete g_instance;
		g_instance = nullptr;
	}
	return;
}


Logger::Logger() : m_levelStr{
	L"INFO",
	L"DEBUG",
	L"WARNING",
	L"EXCEPTION",
	L"ERROR_NORMAL",
	L"ERROR_FATAL",
}
{
}

Logger::~Logger()
{
}

void Logger::Log(Level level, WCHAR * message, int messageLen)
{
#ifdef _DEBUG
	m_mutex.lock();

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::cout << std::put_time(std::localtime(&t), "%F %T");
	std::wcout << L"\t[" << m_levelStr[level] << L"]" << message << std::endl;

	m_mutex.unlock();
#else
	if (level > Level::EXCEPTION)
	{
		m_mutex.lock();

		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::cout << std::put_time(std::localtime(&t), "%F %T");
		std::wcout << L"\t[" << m_levelStr[level] << L"]" << message << std::endl;

		m_mutex.unlock();
	}
#endif
}
