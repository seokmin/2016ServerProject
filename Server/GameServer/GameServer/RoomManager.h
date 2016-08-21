#pragma once
#include <vector>
#include <algorithm>

#include "Room.h"
class User;
class UserManager;

class RoomManager
{
public:
	RoomManager() {};
	~RoomManager() {};

	void Init(UserManager* userManager, PacketQueue* sendPacketQue);
	COMMON::ERROR_CODE LeavUserFromRoom(int roomid, User* pUser);

	std::shared_ptr<Room> GetRoomById(int id);

	ERROR_CODE EnterUser(int sessionIndex);

private:
	std::shared_ptr<Room> GetAvailableRoom();
	std::vector<std::shared_ptr<Room>> m_roomList;
	UserManager* m_pRefUserManager = nullptr;
};

