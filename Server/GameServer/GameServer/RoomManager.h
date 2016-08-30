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

	void Init(UserManager* pUserMan, DBmanager* pDBman, PacketQueue* sendPacketQue);
	COMMON::ERROR_CODE LeavUserFromRoom(int roomid, User* pUser);

	std::shared_ptr<Room> GetRoomById(int id);

	ERROR_CODE EnterUser(int sessionIndex);
	Room* GetRoomByRoomId(int roomId) { return m_roomList[roomId].get(); };

	std::shared_ptr<Room> GetRoomBySessionIndex(int sessioIdx);

	void RunPostTimeAction();



private:
	std::shared_ptr<Room> GetAvailableRoom();
	std::vector<std::shared_ptr<Room>> m_roomList;
	UserManager* m_pRefUserManager = nullptr;
};

