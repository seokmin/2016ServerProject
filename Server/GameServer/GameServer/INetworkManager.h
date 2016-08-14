#pragma once

#include "NetworkConfig.h"
#include "Common.h"

using namespace COMMON;
using NET_ERROR_CODE=NetworkConfig::NET_ERROR_CODE;
class ILog;

class INetworkManager
{
public:
	INetworkManager() {};
	virtual ~INetworkManager() {};

	virtual NET_ERROR_CODE Init(const NetworkConfig* pConfig, ILog* pLogger) { return NET_ERROR_CODE::NONE; }
	
	virtual NET_ERROR_CODE SendData(const int sessionIndex, const short packetId, const short size, const char* pMsg)
	{return NET_ERROR_CODE::NONE;}

	virtual void Run() {}

	virtual RecvPacketInfo GetPacketInfo() { return RecvPacketInfo(); }
};