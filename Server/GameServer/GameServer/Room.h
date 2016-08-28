#pragma once

#include "ServerConfig.h"
#include "Dealer.h"
#include <chrono>

using namespace std::chrono;

class User;
class PacketQueue;

enum class ROOM_STATE
{
	NONE = 0,
	WAITING = 1,
	INGAME = 2,
};

class Room
{
	using PacketInfo = RecvPacketInfo;

public:
	Room(const int i) : m_id(i) {};
	~Room() {};

	void Init(PacketQueue* sendPacketQue);

	bool EnterUser(User* user);

	int GetAvailableSeat();

	bool CheckUserExist(int usrId);

	ERROR_CODE LeaveRoom(User* pUser);

	void NotifyEnterUserInfo(int sessionIndex);
	void NotifyLeaveUserInfo(int sessionIndex);
	void NotifyStartBettingTimer();
	void NotifyBetDone(int sessionIdx, int betMoney);
	void NotifyStartGame();
	void NotifyChangeTurn();

	void SetRoomStateToWaiting();
	COMMON::DealerInfo GetDealerInfo();
	ERROR_CODE ApplyBet(int sessionIndex, int betMoney);
	
	int GetCurrentUserCount() { return m_currentUserCount; };
	User* GetUserInfo(int seatNum) { return m_userList[seatNum]; };
	ROOM_STATE GetCurrentRoomState() { return m_currentRoomState; };
	int GetRoomId() { return m_id; };
	int GetLastActionTime() { return m_lastActionTime; };


private:
	int				m_id;
	User*			m_userList[ServerConfig::MAX_USERCOUNT_PER_ROOM] = {nullptr, };
	int				m_currentUserCount = 0;
	PacketQueue*	m_pSendPacketQue = nullptr;
	unsigned int	m_lastActionTime;

	ROOM_STATE		m_currentRoomState = ROOM_STATE::NONE;
	Dealer			m_dealer;

private:
	User* GetUserBySessionIndex(int sessionIndex);
	int GetUserSeatBySessionIndex(int sessionIndex);
};