#pragma once
#include "ServerConfig.h"
class User;

class Room
{
public:
	Room(const int i);
	~Room();

private:
	int m_id;
	User* m_userList[NetworkConfig::MAX_USERCOUNT_PER_ROOM] = {nullptr, };
};

