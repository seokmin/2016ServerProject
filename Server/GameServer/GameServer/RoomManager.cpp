#include "stdafx.h"
#include "RoomManager.h"
#include "UserManager.h"
#include "PacketQueue.h"

void RoomManager::Init(UserManager* userManager, PacketQueue* sendPacketQue)
{
	m_pRefUserManager = userManager;

	for (int i = 0; i < ServerConfig::MAX_ROOMCOUNT; i++)
	{
		auto room = std::make_shared<Room>(i);
		room->Init(sendPacketQue);
		m_roomList.push_back(room);
	}
}

ERROR_CODE RoomManager::EnterUser(int sessionIndex)
{
	std::shared_ptr<Room> targetRoom = GetAvailableRoom();
	if (targetRoom == nullptr)
	{
		printf_s("유저(%d)가 들어갈 수 있는 방이 없습니다. \n", sessionIndex);
		return ERROR_CODE::ROOM_ENTER_CHANNEL_FULL;
	}

	auto targetUser = m_pRefUserManager->GetUserBySessionIndex(sessionIndex);
	if (targetUser == nullptr)
	{
		printf_s("해당 세션의 유저(%d)가 없어서 방에 들어갈 수 없습니다. \n", sessionIndex);
		return ERROR_CODE::ROOM_ENTER_INVALID_SESSION_INDEX;
	}

	if (!targetRoom->EnterUser(targetUser.get()))
	{
		return ERROR_CODE::ROOM_ENTER_ROOM_FULL;
	}

	return ERROR_CODE::NONE;
}

std::shared_ptr<Room> RoomManager::GetAvailableRoom()
{
	int maxUserNum = ServerConfig::MAX_USERCOUNT_PER_ROOM;
	auto iter = std::find_if(std::begin(m_roomList), std::end(m_roomList), [maxUserNum](auto pRoom) {return pRoom->GetCurrentUserCount() > 0 && pRoom->GetCurrentUserCount() < maxUserNum; });
	if (iter != std::end(m_roomList))
		return *iter;

	iter = std::find_if(std::begin(m_roomList), std::end(m_roomList), [](auto pRoom) {return pRoom->GetCurrentUserCount() == 0; });
	if (iter != std::end(m_roomList))
		return *iter;

	return nullptr;
}

COMMON::ERROR_CODE RoomManager::LeavUserFromRoom(int roomid, User * pUser)
{
	return m_roomList[roomid]->LeaveRoom(pUser);
}

std::shared_ptr<Room> RoomManager::GetRoomById(int id)
{
	return m_roomList[id];
}
