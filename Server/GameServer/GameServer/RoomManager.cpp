#include "stdafx.h"
#include "RoomManager.h"

void RoomManager::Init()
{
	for (int i = 0; i < ServerConfig::MAX_ROOMCOUNT; i++)
	{
		auto room = std::make_shared<Room>(i);
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
