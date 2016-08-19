#include "stdafx.h"
#include "RoomManager.h"
#include "NetworkConfig.h"

RoomManager::RoomManager()
{
}


RoomManager::~RoomManager()
{
}

void RoomManager::init()
{
	for (int i = 0; i < NetworkConfig::MaxRoomCount; i++)
	{
		auto room = std::make_shared<Room>(i);
		m_roomList.push_back(room);
	}

}
