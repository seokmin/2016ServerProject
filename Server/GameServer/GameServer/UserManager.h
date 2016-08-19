#pragma once
#include <vector>
class User;

class UserManager
{
public:
	UserManager();
	~UserManager();

	int GetCurrentUserCount();

private:
	std::vector<User*> m_userList;
};

