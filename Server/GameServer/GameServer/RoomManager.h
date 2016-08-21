#pragma once
#include <vector>
#include <algorithm>

#include "Room.h"

class RoomManager
{
public:
	RoomManager() {};
	~RoomManager() {};

	void Init();

	ERROR_CODE EnterUser(int sessionIndex);

private:
	std::shared_ptr<Room> GetAvailableRoom();
	std::vector<std::shared_ptr<Room>> m_roomList;
};

