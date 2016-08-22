#include "stdafx.h"
#include "UserManager.h"
#include "Room.h"
#include "RoomManager.h"
#include <algorithm>

void UserManager::Init(RoomManager* roomMgr)
{
	for (int i = 0; i < ServerConfig::MAX_USERCOUNT_PER_CHANNEL; ++i)
	{
		auto user = std::make_shared<User>(i);
		m_userList.push_back(user);
	}
	m_roomMgr = roomMgr;
}

bool UserManager::LoginUser(const int sessionIndex, std::string authToken)
{
	//TODO: DB에서 체크해야 함.

	std::shared_ptr<User> newUser = GetAvailableUserFromPool();
	if (newUser == nullptr)
		return false;

	newUser->Init(sessionIndex, authToken);
	++m_curUserCount;

	return true;
}

bool UserManager::LogoutUser(const int sessionIndex)
{
	if (sessionIndex < 0 || sessionIndex >= m_userList.size())
		return false;

	std::shared_ptr<User> user = m_userList.at(sessionIndex);
	
	auto roomId = user->GetCurRoomIdx();
	auto ret = m_roomMgr->LeavUserFromRoom(roomId, user.get());

	user->Clear(); //delete user infomation
	--m_curUserCount;

	if (ret != ERROR_CODE::NONE)
	{
		WCHAR errStr[100];
		wsprintf(errStr, L"방에 그런 사람 없습니다. RoomId:%d UserId:%d\n", roomId, user->GetUserIdx());
		Logger::GetInstance()->Log(Logger::WARNING, errStr, 100);

		return false;
	}
	
	return true;
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

std::shared_ptr<User> UserManager::GetUserBySessionId(int sessionIdx)
{
	auto iter = std::find_if(std::begin(m_userList), std::end(m_userList), [sessionIdx](auto pUser) {return pUser->GetUserSessionIdx() == sessionIdx; });
	if (iter == std::end(m_userList))
		return nullptr;
	return *iter;
}



