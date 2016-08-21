#include "stdafx.h"

#include "User.h"
#include "IOCPManager.h"
#include "PacketQueue.h"

#include "Room.h"

void Room::Init(PacketQueue* sendPacketQue)
{
	m_pSendPacketQue = sendPacketQue;
}

bool Room::EnterUser(User* user)
{
	int seat = GetAvailableSeat();
	if (seat == -1)
	{
		printf_s("방(%d)에 들어왔는데 자리가 없음.. \n", m_id);
		return false;
	}
	user->EnterRoom(m_id);
	m_userList[seat] = user;
	++m_currentUserCount;

	return true;
}

int Room::GetAvailableSeat()
{
	for (int i = 0; i < ServerConfig::MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr)
			return i;
	}
	return -1;
}

COMMON::ERROR_CODE Room::LeaveRoom(User * pUser)
{
	int targetPos = -1;

	//find user
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM ; i++)
	{
		if (m_userList[i] == pUser)
		{
			targetPos = i;
			break;
		}
	}

	//Kick out User
	if (targetPos == -1)
		return COMMON::ERROR_CODE::ROOM_LEAVE_NOT_MEMBER;
	else
		m_userList[targetPos] = nullptr; 

	return	COMMON::ERROR_CODE::NONE;
}

// sessionIndex = 들어온 본인 -> 빼고 나머지한테 보냄
void Room::NotifyEnterUserInfo(int sessionIndex)
{
	auto enterUser = GetUserBySessionIndex(sessionIndex);

	PacketRoomEnterNtf pkt;
	pkt._enterUser = enterUser->GetUserInfo();
	pkt._slotNum = GetUserSeatBySessionIndex(sessionIndex);

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;

		if (m_userList[i]->CheckUserWithSessionIndex(sessionIndex))
			continue;

		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::ROOM_ENTER_USER_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}

}

User * Room::GetUserBySessionIndex(int sessionIndex)
{
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] != nullptr)
		{
			if (m_userList[i]->CheckUserWithSessionIndex(sessionIndex))
				return m_userList[i];
		}
	}
}

int Room::GetUserSeatBySessionIndex(int sessionIndex)
{
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] != nullptr)
		{
			if (m_userList[i]->CheckUserWithSessionIndex(sessionIndex))
				return i;
		}
	}
}
