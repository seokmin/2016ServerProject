#pragma once
class User;

#include "ServerConfig.h"

class Room
{
public:
	Room(const int i) : m_id(i) {};
	~Room() {};

	bool EnterUser(User* user);

	int GetAvailableSeat();

	int GetCurrentUserCount() { return m_currentUserCount; };

	COMMON::ERROR_CODE LeaveRoom(User* pUser);

private:
	int m_id;
	User* m_userList[ServerConfig::MAX_USERCOUNT_PER_ROOM] = {nullptr, };
	int m_currentUserCount = 0;

};