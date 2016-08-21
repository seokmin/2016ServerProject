#include "stdafx.h"
#include "UserManager.h"
#include <algorithm>

void UserManager::Init()
{
	for (int i = 0; i < ServerConfig::MAX_USERCOUNT_PER_CHANNEL; ++i)
	{
		auto user = std::make_shared<User>(i);
		m_userList.push_back(user);
	}
}

bool UserManager::LoginUser(const int sessionIndex, std::string authToken)
{
	std::shared_ptr<User> newUser = GetAvailableUserFromPool();
	if (newUser == nullptr)
		return false;

	newUser->Init(sessionIndex, authToken);
	++m_curUserCount;
}

int UserManager::GetCurrentUserCount()
{
	return m_curUserCount;
}

std::shared_ptr<User> UserManager::GetAvailableUserFromPool()
{
	auto iter = std::find_if(std::begin(m_userList), std::end(m_userList), [](auto pUser) {return pUser->IsAvailableFromPool(); });
	if (iter == std::end(m_userList))
		return nullptr;

	return *iter;
}

std::shared_ptr<User> UserManager::GetUserByAuthToken(std::string authToken)
{
	auto iter = std::find_if(std::begin(m_userList), std::end(m_userList), [authToken](auto pUser) {return pUser->CheckUserWithAuthToken(authToken); });
	if (iter == std::end(m_userList))
		return nullptr;

	return *iter;
}



