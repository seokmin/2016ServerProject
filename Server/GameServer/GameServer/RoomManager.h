#pragma once
#include <vector>

#include "Room.h"
class User;

class RoomManager
{
public:
	RoomManager() {};
	~RoomManager() {};

	void Init();
	COMMON::ERROR_CODE LeavUserFromRoom(int roomid, User* pUser);

private:
	std::vector<std::shared_ptr<Room>> m_roomList;
};

