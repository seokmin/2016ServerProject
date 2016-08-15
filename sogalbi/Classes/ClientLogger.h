#pragma once
class ClientLogger
{
public:
	static void logThreadSafe(const char* message);
	static void msgBox(const wchar_t* message, bool threaded = false);
};

