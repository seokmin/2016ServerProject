#pragma once

#include "ServerConfig.h"

class User;
class PacketQueue;

class Room
{
	using PacketInfo = RecvPacketInfo;

public:
	Room(const int i) : m_id(i) {};
	~Room() {};

	void Init(PacketQueue* sendPacketQue);

	bool EnterUser(User* user);

	int GetAvailableSeat();

	int GetCurrentUserCount() { return m_currentUserCount; };
	bool CheckUserExist(int usrId);

	ERROR_CODE LeaveRoom(User* pUser);

	void NotifyEnterUserInfo(int sessionIndex);


	User* GetUserInfo(int seatNum) { return m_userList[seatNum]; };
private:
	int m_id;
	User* m_userList[ServerConfig::MAX_USERCOUNT_PER_ROOM] = {nullptr, };
	int m_currentUserCount = 0;
	PacketQueue* m_pSendPacketQue = nullptr;


private:
	User* GetUserBySessionIndex(int sessionIndex);
	int GetUserSeatBySessionIndex(int sessionIndex);

};