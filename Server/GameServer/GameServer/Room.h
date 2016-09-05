#pragma once

#include "ServerConfig.h"
#include "Dealer.h"
#include <chrono>

using namespace std::chrono;

class User;
class PacketQueue;
class DBmanager;
class ServerConfig;

enum class ROOM_STATE
{
	NONE = 0,
	WAITING = 1,
	HANDOUT = 2,
	INGAME = 3,
	CALCULATE = 4,
};

class Room
{
	using PacketInfo = RecvPacketInfo;

public:
	Room(const int i) : m_id(i) {};
	~Room() {};

	void Init(PacketQueue* sendPacketQue, DBmanager* pDBman, ServerConfig* serverConfig);

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
	void NotifyGameChoice(int sessionIndex, ChoiceKind choice);

	void SetRoomStateToWaiting();
	COMMON::DealerInfo GetDealerInfo();
	ERROR_CODE ApplyBet(int sessionIndex, int betMoney);
	User* GetCurrentBettingUser();
	void ForceNextTurn(int seat, int hand);
	void ForceBetting();

	void EndOfGame();
	void ResetForNextGame();

	ERROR_CODE ApplyChoice(int sessionIndex, ChoiceKind choice);

	int GetCurrentUserCount() { return m_currentUserCount; };
	User* GetUserInfo(int seatNum) { return m_userList[seatNum]; };
	ROOM_STATE GetCurrentRoomState() { return m_currentRoomState; };
	int GetRoomId() { return m_id; };
	unsigned int GetLastActionTime() { return m_lastActionTime; };

	std::tuple<int, int> GetNextPlayerSeatAndHand();
	int GetWaitingForRestart() { return m_waitForRestart; };


private:
	int				m_id;
	User*			m_userList[ServerConfig::MAX_USERCOUNT_PER_ROOM] = {nullptr, };
	int				m_currentUserCount = 0;
	PacketQueue*	m_pSendPacketQue = nullptr;
	unsigned int	m_lastActionTime;

	ROOM_STATE		m_currentRoomState = ROOM_STATE::NONE;
	Dealer			m_dealer;

	int				m_waitForRestart = 100;
	DBmanager*		m_pDBmanager;

	ServerConfig*	m_pServerConfig;

private:
	User* GetUserBySessionIndex(int sessionIndex);
	int GetUserSeatBySessionIndex(int sessionIndex);
	int CalculateEarnMoney(Dealer* pDealer, User* pUser);
	int CalculateResultByHand(HandInfo dHand, HandInfo uHand, int betMoney);
};