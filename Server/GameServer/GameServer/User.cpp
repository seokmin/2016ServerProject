#include "stdafx.h"
#include "User.h"

void User::Init(int sessionIndex, std::string authToken)
{
	m_sessionIndex = sessionIndex;
	m_authToken = authToken;
	m_curDomain = DOMAIN_STATE::LOGIN;
}

void User::Clear()
{
	m_sessionIndex = -1;
	m_authToken = "";
	m_currentRoomIdx = -1;
	m_curDomain = DOMAIN_STATE::NONE;
}
