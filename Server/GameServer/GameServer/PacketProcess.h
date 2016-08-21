#pragma once

#include "INetworkManager.h"
#include "Common.h"
#include "ServerConfig.h"

using namespace COMMON;
class ITcpNetwork;
class ILog;

class UserManager;
class LobbyManager;
class User;

#define CHECK_START  ERROR_CODE __result=ERROR_CODE::NONE;
#define CHECK_ERROR(f) __result=f; goto CHECK_ERR;

class PacketProcess
{
	using PacketInfo = RecvPacketInfo;
	typedef ERROR_CODE(PacketProcess::*PacketFunc)(PacketInfo);
	using TcpNet = ITcpNetwork;
	PacketFunc PacketFuncArray[(int)PACKET_ID::MAX];

public:
	PacketProcess() {};
	~PacketProcess() {};

	void Init(TcpNet* pNetwork, UserManager* pUserMgr, LobbyManager* pLobbyMgr, ILog* pLogger);

	void Process(PacketInfo packetInfo);

private:
	TcpNet* m_pRefNetwork;
	UserManager* m_pRefUserMgr;
	LobbyManager* m_pRefLobbyMgr;
	ILog* m_pRefLogger;

private:
	ERROR_CODE NtfSysCloseSesson(PacketInfo packetInfo);

	ERROR_CODE RoomEnter(PacketInfo packetInfo);
	ERROR_CODE RoomUserList(PacketInfo packetInfo);
	ERROR_CODE RoomLeave(PacketInfo packetInfo);
	ERROR_CODE RoomChat(PacketInfo packetInfo);
	ERROR_CODE RoomChange(PacketInfo packetInfo);
	
	ERROR_CODE LeaveAllAndLogout(User* pUser, int SessionId);
};