#include "stdafx.h"
#include "Room.h"
#include "User.h"

bool Room::EnterUser(User* user)
{
	int seat = GetAvailableSeat();
	if (seat == -1)
	{
		printf_s("방(%d)에 들어왔는데 자리가 없음.. \n", m_id);
		return false;
	}
	user->EnterRoom(m_id);
	m_userList[seat] = user;
	++m_currentUserCount;

	return true;
}

int Room::GetAvailableSeat()
{
	for (int i = 0; i < ServerConfig::MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr)
			return i;
	}
		return -1;
}
