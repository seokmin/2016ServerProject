#pragma once
#include <vector>

#include "User.h"

class UserManager
{
public:
	UserManager();
	~UserManager();

	void Init(ServerConfig*);

	void LoginUser(const char* authToken);

	int GetCurrentUserCount();
	std::shared_ptr<User> GetAvailableUserPool();

	

private:
	std::vector<std::shared_ptr<User>> m_userList;
	ServerConfig* m_pRefServerConfig = nullptr;
};

