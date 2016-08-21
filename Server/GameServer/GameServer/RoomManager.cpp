#include "stdafx.h"
#include "RoomManager.h"

RoomManager::RoomManager()
{
}


RoomManager::~RoomManager()
{
}

void RoomManager::Init()
{
	for (int i = 0; i < ServerConfig::MAX_ROOMCOUNT; i++)
	{
		auto room = std::make_shared<Room>(i);
		m_roomList.push_back(room);
	}

}
