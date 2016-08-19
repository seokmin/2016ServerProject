#pragma once
class ClientLogger
{
public:
	static void logThreadSafe(const char* message);
	static void msgBox(const wchar_t* message, const wchar_t* title = L"JackBlack", bool threaded = false);
};

