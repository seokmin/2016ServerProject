#include "stdafx.h"
#include "IOCPManager.h"


IOCPManager* IOCPManager::_instance = nullptr;

void IOCPManager::WorkerThreadFunc()
{

}

void IOCPManager::ListenThreadFunc()
{

}

void IOCPManager::InitServer(ServerSetting& setting)
{
	// 윈속 초기화
	auto wsaData = WSADATA{};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		Beep(1000,1000); // TODO : 에러 발생시 일단 beep 해두었으나 


	// IOCP를 만든다.
	_completionPort = CreateIOCP();

	// worker 스레드 풀을 만든다.
	auto sysInfo = SYSTEM_INFO{};
	GetSystemInfo(&sysInfo); // 시스템 정보 알아오기. 코어 개수 파악을 위함
	auto numThread = sysInfo.dwNumberOfProcessors*2; // 코어 개수 * 2개만큼 worker 스레드 풀을 만든다.
	for (auto i = 0u; i < numThread; ++i)
	{
		//std::thread(WorkerThreadFunc);
	}


	// 서버 소켓 셋팅
	// TODO : SOCK_STREAM, WSA_FLAG_OVERLAPPED의 의미는 잘 모르겠다. 주석 바람
	_serverSocket = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
	sockaddr_in socketAddress;
	ZeroMemory(&socketAddress, sizeof(socketAddress));
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddress.sin_port = htons(setting._portNum);
	bind(_serverSocket, (sockaddr*)&socketAddress, sizeof(socketAddress));

}

HANDLE IOCPManager::CreateIOCP()
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
}

void IOCPManager::BindSocketToIOCP()
{
	
}

IOCPManager* IOCPManager::GetInstance()
{
	if (_instance == nullptr)
		_instance = new IOCPManager();
	return _instance;
}

void IOCPManager::StartServer()
{
	
}
