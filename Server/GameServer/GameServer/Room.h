#pragma once
class User;

#include "ServerConfig.h"

class Room
{
public:
	Room(const int i) : m_id(i) {};
	~Room() {};

private:
	int m_id;
	User* m_userList[ServerConfig::MAX_USERCOUNT_PER_ROOM] = {nullptr, };
};