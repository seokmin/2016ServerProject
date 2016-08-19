#pragma once
#include "Common.h"
#include <vector>
#include "Room.h"

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

	void init();

private:
	std::vector<std::shared_ptr<Room>> m_roomList;
};

