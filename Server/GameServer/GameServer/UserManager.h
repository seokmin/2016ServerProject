#pragma once
#include <vector>

#include "User.h"
class RoomManager;

class UserManager
{
public:
	UserManager() {};
	~UserManager() {};

	void Init(RoomManager* roomMgr);

	bool LoginUser(const int sessionIndex, std::string authToken);
	bool LogoutUser(const int sessionIndex);

	std::shared_ptr<User> GetUserBySessionIndex(int sessionIndex);

	int GetCurrentUserCount();

private:
	std::shared_ptr<User> GetAvailableUserFromPool();
	std::shared_ptr<User> GetUserByAuthToken(std::string authToken);

private:
	std::vector<std::shared_ptr<User>> m_userList;

	int m_curUserCount = 0;
	
	RoomManager* m_roomMgr;
};

