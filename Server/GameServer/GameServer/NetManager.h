#pragma once
class NetManager : INetworkManager
{
public:
	NET_ERROR_CODE Init(const NetworkConfig* pConfig, ILog* pLogger) override;

	NET_ERROR_CODE SendData(const int sessionIndex, const short packetId, const short size, const char* pMsg) override;

	void Run() override;

	virtual RecvPacketInfo GetPacketInfo() { return RecvPacketInfo(); }
private:
	NetManager() {};
public:
private:
};