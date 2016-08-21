#include "stdafx.h"
#include "Logger.h"
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string


Logger::Logger() : m_levelStr{
	L"INFO",
	L"DEBUG",
	L"WARNING",
	L"EXCEPTION",
	L"ERROR_NORMAL",
	L"ERROR_SIMPLE",
}
{
}


Logger::~Logger()
{
}

void Logger::Log(Level level, WCHAR * message, int lessageLen)
{

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::cout << std::put_time(std::localtime(&t), "%F %T");
	std::wcout << L"\t[" << m_levelStr[level] << L"]" << message << std::endl;
}
