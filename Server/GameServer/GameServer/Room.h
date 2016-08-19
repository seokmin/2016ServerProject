#pragma once
#include "NetworkConfig.h"
class User;

class Room
{
public:
	Room(const int i);
	~Room();

private:
	int m_id;
	User* m_userList[NetworkConfig::MaxRoomUserCount] = {nullptr, };
};

