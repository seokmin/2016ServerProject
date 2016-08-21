#include "stdafx.h"
#include "PacketProcess.h"

#include "User.h"
#include "UserManager.h"



void PacketProcess::Init(TcpNet * pNetwork, UserManager * pUserMgr, LobbyManager * pLobbyMgr, ILog * pLogger)
{
	m_pRefNetwork = pNetwork;
	m_pRefUserMgr = pUserMgr;
	m_pRefLobbyMgr = pLobbyMgr;
	m_pRefLogger = pLogger;

	for (int i = 0; i < PACKET_ID::MAX; ++i)
	{
		PacketFuncArray[i] = nullptr;
	}

	PacketFuncArray[(int)NetworkConfig::PACKET_ID::NTF_SYS_CLOSE_SESSION] = &PacketProcess::NtfSysCloseSesson;
	PacketFuncArray[(int)PACKET_ID::ROOM_ENTER_REQ] = &PacketProcess::RoomEnter;
}

void PacketProcess::Process(PacketInfo packetInfo)
{
	auto packetId = packetInfo.PacketId;

	if (PacketFuncArray[packetId] == nullptr)
	{
		printf_s("존재하지 않는 ID의 패킷입니다. 위치 : %s \n", __FUNCTION__);
		return;
	}
	
	(this->*PacketFuncArray[packetId])(packetInfo);
}

ERROR_CODE PacketProcess::NtfSysCloseSesson(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomEnter(PacketInfo packetInfo)
{
	auto reqPkt = (PacketRoomEnterReq*)packetInfo.pRefData;
	PacketRoomEnterRes resPkt;

	m_pRefUserMgr->LoginUser(reqPkt->_authToken);

	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomUserList(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomLeave(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomChat(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomChange(PacketInfo packetInfo)
{
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::LeaveAllAndLogout(User * pUser, int SessionId)
{
	//[TODO] ...
	return ERROR_CODE();
}