#pragma once

#include "Common.h"
#include "ServerConfig.h"
#include "PacketQueue.h"

using namespace COMMON;
class ITcpNetwork;
class ILog;

class UserManager;
class User;

class RoomManager;

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

	void Init(UserManager* pUserMgr, RoomManager* pRoomMgr, PacketQueue* pRecvPacketQue, PacketQueue* pSendPacketQue);

	void Process(PacketInfo packetInfo);

private:
	UserManager* m_pRefUserMgr;
	RoomManager* m_pRefRoomMgr;
	PacketQueue* m_pSendPacketQue;
	PacketQueue* m_pRecvPacketQue;

private:
	ERROR_CODE NtfSysCloseSesson(PacketInfo packetInfo);

	ERROR_CODE RoomEnter(PacketInfo packetInfo);
	ERROR_CODE RoomUserList(PacketInfo packetInfo);
	ERROR_CODE RoomLeave(PacketInfo packetInfo);
	ERROR_CODE RoomChat(PacketInfo packetInfo);
	ERROR_CODE RoomChange(PacketInfo packetInfo);

	ERROR_CODE GameBet(PacketInfo packetInfo);
	ERROR_CODE GameChoice(PacketInfo packetInfo);
	
private:
};