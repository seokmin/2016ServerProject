#include "stdafx.h"
#include "IOCPManager.h"


IOCPManager* _instance = nullptr;
IOCPManager* IOCPManager::GetInstance()
{
	if (_instance == nullptr)
		_instance = new IOCPManager();
	return _instance;
}

void IOCPManager::Initialize()
{

}