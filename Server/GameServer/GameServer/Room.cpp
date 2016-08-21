#include "stdafx.h"
#include "Room.h"
#include "User.h"

COMMON::ERROR_CODE Room::LeaveRoom(User * pUser)
{
	int targetPos = -1;
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM ; i++)
	{
		if (m_userList[i] == pUser)
		{
			targetPos = i;
			break;
		}
	}
	if (targetPos == -1)
		return COMMON::ERROR_CODE::ROOM_LEAVE_NOT_MEMBER;
	else
		m_userList[targetPos] = nullptr;

	return	COMMON::ERROR_CODE::NONE;
}
