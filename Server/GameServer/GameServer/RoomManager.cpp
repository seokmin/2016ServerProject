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

COMMON::ERROR_CODE RoomManager::LeavUserFromRoom(int roomid, User * pUser)
{
	return m_roomList[roomid]->LeaveRoom(pUser);
}
