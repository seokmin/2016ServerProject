#include "stdafx.h"
#include "RoomManager.h"
#include "ServerConfig.h"

RoomManager::RoomManager()
{
}


RoomManager::~RoomManager()
{
}

void RoomManager::init()
{
	for (int i = 0; i < NetworkConfig::MAX_ROOMCOUNT; i++)
	{
		auto room = std::make_shared<Room>(i);
		m_roomList.push_back(room);
	}

}
