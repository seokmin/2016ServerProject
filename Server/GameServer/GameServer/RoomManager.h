#pragma once
#include <vector>

#include "Room.h"

class RoomManager
{
public:
	RoomManager();
	~RoomManager();

	void Init();

private:
	std::vector<std::shared_ptr<Room>> m_roomList;
};

