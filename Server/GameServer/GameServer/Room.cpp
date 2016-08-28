#include "stdafx.h"

#include "User.h"
#include "IOCPManager.h"
#include "PacketQueue.h"

#include "Room.h"
#include "Dealer.h"

void Room::Init(PacketQueue* sendPacketQue)
{
	m_pSendPacketQue = sendPacketQue;

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		m_userList[i] = nullptr;
	}
}

bool Room::EnterUser(User* user)
{
	int seat = GetAvailableSeat();
	if (seat == -1)
	{
		printf_s("방(%d)에 들어왔는데 자리가 없음.. \n", m_id);
		return false;
	}
	user->EnterRoom(m_id);
	user->SetCurSeat(seat);
	m_userList[seat] = user;
	++m_currentUserCount;

	// 현재 방의 시작 요청 시간을 기록한다. 노티는 user list req 까지 오면 보낸다.
	if (m_currentRoomState == ROOM_STATE::NONE || m_currentRoomState == ROOM_STATE::WAITING)
	{
		SetRoomStateToWaiting();
	}
	else
	{
		// 만약 이미 게임중이면 들어온 유저는 waiting으로 바꿔준다.
		user->SetGameState(GAME_STATE::WAITING);
	}
	
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

bool Room::CheckUserExist(int sessionIdx)
{
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr)
			continue;

		if (m_userList[i]->GetSessionIndex() == sessionIdx)
			return true;
	}

	return false;
}

COMMON::ERROR_CODE Room::LeaveRoom(User * pUser)
{
	// Notify
	NotifyLeaveUserInfo(pUser->GetSessionIndex());

	int targetPos = -1;

	//find user
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM ; ++i)
	{
		if (m_userList[i] == pUser)
		{
			targetPos = i;
			break;
		}
	}

	//Kick out User
	if (targetPos == -1)
		return COMMON::ERROR_CODE::ROOM_LEAVE_NOT_MEMBER;
	else
		m_userList[targetPos] = nullptr; 
	
	// 유저의 상태/방정보도 나가도록 해줘야 함
	//pUser->LeaveRoom();

	--m_currentUserCount;
	if (m_currentUserCount == 0)
		m_currentRoomState = ROOM_STATE::NONE;

	WCHAR infoStr[100];
	wsprintf(infoStr, L"유저가 로그아아웃 했습니다. RoomId:%d UserName:%s", m_id, pUser->GetName().c_str());
	Logger::GetInstance()->Log(Logger::INFO, infoStr, 100);

	pUser->Clear();

	return	COMMON::ERROR_CODE::NONE;
}

void Room::NotifyStartBettingTimer()
{
	// 방의 상태가  waiting이 아니면 노티를 보내면 안됨
	if (m_currentRoomState != ROOM_STATE::WAITING)
		return;
	
	PacketGameBetCounterNtf pkt;
	pkt._countTime = 10;
	pkt.minBet = ServerConfig::minBet;
	pkt.maxBet = ServerConfig::maxBet;

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;

		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::GAME_BET_COUNTER_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}
}

void Room::NotifyBetDone(int sessionIndex, int betMoney)
{
	PacketGameBetNtf pkt;

	pkt._betMoney = betMoney;
	pkt._betSlot = GetUserSeatBySessionIndex(sessionIndex);

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;

		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::GAME_BET_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}
}

void Room::SetRoomStateToWaiting()
{
	m_currentRoomState = ROOM_STATE::WAITING;
	m_lastActionTime = duration_cast< milliseconds >(
		steady_clock::now().time_since_epoch()
		).count();

	// 유저는 이제 베팅을 해야하므로 betting으로 바꿔준다.
	for (auto& user : m_userList)
	{
		if (user == nullptr)
			continue;

		user->SetGameState(GAME_STATE::BETTING);
	}
}

COMMON::DealerInfo Room::GetDealerInfo()
{
	COMMON::DealerInfo dInfo;
	
	for (int i = 0; i < 8; ++i)
	{
		dInfo._openedCardList[i] = m_dealer.GetCard(i);
	}

	return dInfo;
}

ERROR_CODE Room::ApplyBet(int sessionIndex, int betMoney)
{
	User* user = GetUserBySessionIndex(sessionIndex);

	// 상태가 베팅이 아닌데 감히 베팅을 하려 하다니..
	if (user->GetGameState() != GAME_STATE::BETTING)
	{
		return ERROR_CODE::ROOM_GAME_NOT_IN_PROPER_STATE;
	}

	// 유저의 돈을 갈취한 뒤..
	auto ret = user->ApplyBet(betMoney);
	if (ret != ERROR_CODE::NONE)
		return ret;

	// 표현할 수 있게 베팅 정보를 노티한다.
	NotifyBetDone(sessionIndex, betMoney);

	// 베팅이 들어온 시점에서 방 안의 유저가 모두 베팅완료라면 게임 시작
	bool flag = true;
	for (auto& user : m_userList)
	{
		if (user == nullptr)
		{
			continue;
		}

		if (user->GetGameState() != GAME_STATE::BET_DONE &&
			user->GetGameState() != GAME_STATE::WAITING)
		{
			flag = false;
		}
	}
	if (flag)
	{
		NotifyStartGame();
	}

	return ERROR_CODE::NONE;
}

void Room::NotifyStartGame()
{
	m_currentRoomState = ROOM_STATE::INGAME;

	PacketGameStartNtf pkt;

	bool flag = true;
	for (int i=0; i<MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr)
			continue;
		else if (flag)
		{
			// 가장 앞 슬롯의 유저에게 턴을 주고 시작위치라고 표시한다
			flag = false;
			m_userList[i]->SetGameState(GAME_STATE::ACTIONING);
			pkt._startSlotNum = m_userList[i]->GetCurSeat();
		}
		else
			m_userList[i]->SetGameState(GAME_STATE::ACTION_WAITING);

		// 상태를 다 바꿨으면 실제로 카드를 나눠주기 시작한다.
		m_dealer.Init(this);
	}
	
	//패킷 조제
	pkt._turnCountTime = 10;
	pkt._dealerCard = m_dealer.GetCard(0);

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;

		pkt._handInfo[i] = m_userList[i]->GetHand(0);
	}

	//패킷 전쏭
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;

		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::GAME_START_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}
}

void Room::NotifyChangeTurn()
{
}

// sessionIndex = 들어온 본인 -> 빼고 나머지한테 보냄
void Room::NotifyEnterUserInfo(int sessionIndex)
{
	auto enterUser = GetUserBySessionIndex(sessionIndex);

	PacketRoomEnterNtf pkt;
	pkt._enterUser = enterUser->GetUserInfo();
	pkt._slotNum = GetUserSeatBySessionIndex(sessionIndex);

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;

		if (m_userList[i]->CheckUserWithSessionIndex(sessionIndex))
			continue;

		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::ROOM_ENTER_USER_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}
}

void Room::NotifyLeaveUserInfo(int sessionIndex)
{
	auto leaveUser = GetUserBySessionIndex(sessionIndex);

	PacketRoomLeaveNtf pkt;
	pkt._slotNum = leaveUser->GetCurSeat();

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;

		if (m_userList[i]->CheckUserWithSessionIndex(sessionIndex))
			continue;

		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::ROOM_LEAVE_USER_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}
}

User * Room::GetUserBySessionIndex(int sessionIndex)
{
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] != nullptr)
		{
			if (m_userList[i]->CheckUserWithSessionIndex(sessionIndex))
				return m_userList[i];
		}
	}

	return nullptr;
}

int Room::GetUserSeatBySessionIndex(int sessionIndex)
{
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] != nullptr)
		{
			if (m_userList[i]->CheckUserWithSessionIndex(sessionIndex))
				return i;
		}
	}

	return -1;
}
