#include "stdafx.h"
#include "UserManager.h"
#include "User.h"

UserManager::UserManager()
{
}


UserManager::~UserManager()
{
}

int UserManager::GetCurrentUserCount()
{
	return m_userList.size();
}
