#pragma once
#include <vector>
#include <algorithm>

#include "Room.h"
class User;

class RoomManager
{
public:
	RoomManager() {};
	~RoomManager() {};

	void Init();
	COMMON::ERROR_CODE LeavUserFromRoom(int roomid, User* pUser);

	ERROR_CODE EnterUser(int sessionIndex);
	Room* GetRoomByRoomId(int roomId) { return m_roomList[roomId].get(); };
private:
	std::shared_ptr<Room> GetAvailableRoom();
	std::vector<std::shared_ptr<Room>> m_roomList;
};

