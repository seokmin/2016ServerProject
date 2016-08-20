#pragma once
#include <vector>

#include "User.h"

class UserManager
{
public:
	UserManager();
	~UserManager();

	int GetCurrentUserCount();

private:
	std::vector<User*> m_userList;
};

