#pragma once
class User;

#include "ServerConfig.h"

class Room
{
public:
	Room(const int i) : m_id(i) {};
	~Room() {};

	COMMON::ERROR_CODE LeaveRoom(User* pUser);
private:
	int m_id;
	User* m_userList[ServerConfig::MAX_USERCOUNT_PER_ROOM] = {nullptr, };
};