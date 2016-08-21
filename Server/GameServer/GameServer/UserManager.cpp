#include "stdafx.h"
#include "UserManager.h"

UserManager::UserManager()
{
}


UserManager::~UserManager()
{
}

void UserManager::Init(NetworkConfig* networkConfig)
{
	m_pRefNetworkConfig = networkConfig;

	for (int i = 0; i < m_pRefNetworkConfig->MaxClientCount; ++i)
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
	return m_userList.size();
}

std::shared_ptr<User> UserManager::GetAvailableUserPool()
{
	return std::shared_ptr<User>();
}



