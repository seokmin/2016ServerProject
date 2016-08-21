#include "stdafx.h"
#include "PacketProcess.h"

#include "User.h"
#include "UserManager.h"

#include "Room.h"
#include "RoomManager.h"

#include "IOCPManager.h"



void PacketProcess::Init(UserManager * pUserMgr, RoomManager * pRoomMgr, PacketQueue* pRecvPacketQue, PacketQueue* pSendPacketQue)
{
	m_pRefUserMgr = pUserMgr;
	m_pRefRoomMgr = pRoomMgr;
	m_pRecvPacketQue = pRecvPacketQue;
	m_pSendPacketQue = pSendPacketQue;

	for (int i = 0; i < PACKET_ID::MAX; ++i)
	{
		PacketFuncArray[i] = nullptr;
	}

	PacketFuncArray[(int)ServerConfig::PACKET_ID::NTF_SYS_CLOSE_SESSION] = &PacketProcess::NtfSysCloseSesson;
	PacketFuncArray[(int)PACKET_ID::ROOM_ENTER_REQ] = &PacketProcess::RoomEnter;
	PacketFuncArray[(int)PACKET_ID::ROOM_LEAVE_REQ] = &PacketProcess::RoomLeave;
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

ERROR_CODE PacketProcess::RoomEnter(PacketInfo packetInfo)
{
	auto reqPkt = (PacketRoomEnterReq*)packetInfo.pRefData;
	PacketRoomEnterRes resPkt;

	if (!m_pRefUserMgr->LoginUser(packetInfo.SessionIndex, std::string(reqPkt->_authToken)))
		return ERROR_CODE::ROOM_ENTER_CHANNEL_FULL;

	printf_s("유저(%d)정보가 정상적으로 잘 등록됬음 \n", packetInfo.SessionIndex);

	ERROR_CODE result = m_pRefRoomMgr->EnterUser(packetInfo.SessionIndex);
	if (result != ERROR_CODE::NONE)
		return result;

	printf_s("유저(%d)가 방(%d)에 들어갔음 \n", packetInfo.SessionIndex, m_pRefUserMgr->GetUserBySessionIndex(packetInfo.SessionIndex)->GetCurRoomIdx());

	// 바로 위에서 m_pRefUserMgr->GetUserBySessionIndex(packetInfo.SessionIndex)이 null인지 확인하므로 두 번 확인은 안 함..
	resPkt._roomNum = m_pRefUserMgr->GetUserBySessionIndex(packetInfo.SessionIndex)->GetCurRoomIdx();
	
	auto targetRoom = m_pRefRoomMgr->GetRoomById(resPkt._roomNum);

	// Res 보냄
	PacketInfo sendPacket;
	sendPacket.SessionIndex = packetInfo.SessionIndex;
	sendPacket.PacketId = PACKET_ID::ROOM_ENTER_RES;
	sendPacket.pRefData = (char *)&resPkt;
	sendPacket.PacketBodySize = sizeof(resPkt);
	m_pSendPacketQue->PushBack(sendPacket);
	
	// Notify
	targetRoom->NotifyEnterUserInfo(packetInfo.SessionIndex);

	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::RoomUserList(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomLeave(PacketInfo packetInfo)
{
	auto reqPkt = (PacketRoomLeaveReq*)packetInfo.pRefData;
	PacketRoomLeaveRes resPkt;

	//방에서 나가면 자동으로 로그아웃
	if (!m_pRefUserMgr->LogoutUser(packetInfo.SessionIndex))
	{
		NtfSysCloseSesson(packetInfo);
		return ERROR_CODE::ROOM_LEAVE_NOT_MEMBER;
	}

	resPkt.SetErrCode(ERROR_CODE::NONE);
	
	PacketInfo sendPacketInfo;
	sendPacketInfo.PacketId = PACKET_ID::ROOM_LEAVE_RES;
	sendPacketInfo.SessionIndex = packetInfo.SessionIndex;
	sendPacketInfo.PacketBodySize = sizeof(resPkt);
	sendPacketInfo.pRefData = (char*)&resPkt;
	m_pSendPacketQue->PushBack(sendPacketInfo);

	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomChat(PacketInfo packetInfo)
{
	//???? roomchat 우리 없이 가자고 하지 않음?
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomChange(PacketInfo packetInfo)
{
	return ERROR_CODE();
}


ERROR_CODE PacketProcess::NtfSysCloseSesson(PacketInfo packetInfo)
{
	PacketInfo sendPacketInfo;
	sendPacketInfo.SessionIndex = packetInfo.SessionIndex;
	sendPacketInfo.PacketId = PACKET_ID::CLOSE_SESSION;
	sendPacketInfo.PacketBodySize = 0;
	sendPacketInfo.pRefData = nullptr;
	m_pSendPacketQue->PushBack(packetInfo);
	return ERROR_CODE();
}
