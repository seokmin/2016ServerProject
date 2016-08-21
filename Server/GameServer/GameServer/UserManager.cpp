#include "stdafx.h"
#include "UserManager.h"

UserManager::UserManager()
{
}


UserManager::~UserManager()
{
}

void UserManager::Init(ServerConfig* ServerConfig)
{
	m_pRefServerConfig = ServerConfig;

	for (int i = 0; i < m_pRefServerConfig->MAX_USERCOUNT_PER_CHANNEL; ++i)
	{
		auto user = std::make_shared<User>(i);
		m_userList.push_back(user);
	}
}

void UserManager::LoginUser(const char * authToken)
{

}

int UserManager::GetCurrentUserCount()
{
	return (int)m_userList.size();
}

std::shared_ptr<User> UserManager::GetAvailableUserPool()
{
	return std::shared_ptr<User>();
}



