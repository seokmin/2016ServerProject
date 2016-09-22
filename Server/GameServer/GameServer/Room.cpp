#include "stdafx.h"

#include "User.h"
#include "IOCPManager.h"
#include "PacketQueue.h"
#include "DBmanager.h"
#include "ServerConfig.h"

#include "Room.h"
#include "Dealer.h"
#include <cassert>

void Room::Init(PacketQueue* sendPacketQue, DBmanager* pDBman, ServerConfig* serverConfig)
{
	m_pSendPacketQue = sendPacketQue;
	m_pDBmanager = pDBman;
	m_pServerConfig = serverConfig;

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		m_userList[i] = nullptr;
	}
}

Room::~Room() {
	for (auto& pUser : m_userList) {
		LeaveRoom(pUser);
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

	//find user
	int targetPos = -1;
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
	
	--m_currentUserCount;
	if (m_currentUserCount == 0)
	{
		m_currentRoomState = ROOM_STATE::NONE;
		m_waitForRestart = 100; // 초기화용..
		m_dealer.Clear();
	}
	else if (pUser->GetGameState() == GAME_STATE::ACTIONING) //유저가 게임중이었으면 다음 사람으로 진행
	{
		//turn 진행을 알림.
		pUser->SetGameState(GAME_STATE::ACTION_DONE);
		Logger::GetInstance()->Logf(Logger::Level::INFO, L"User(%s)'s Current Hand state : %d", pUser->GetName().c_str(), (int)pUser->GetHand(0)._handState);
		//pUser->SetHandState(0, HandInfo::HandState::BURST);

		NotifyChangeTurn();
	}

	// 유저의 상태/방정보도 나가도록 해줘야 함
	Logger::GetInstance()->Logf(Logger::INFO, L"유저가 로그아아웃 했습니다. RoomId:%d UserName:%s", m_id, pUser->GetName().c_str());
	// Auth Token도 지워줌니다.
	m_pDBmanager->DeleteAuthToken(pUser);
	
	pUser->LeaveRoom(); //정보 삭제는 NTFclose session 에서 한다.

	return	COMMON::ERROR_CODE::NONE;
}

void Room::NotifyStartBettingTimer()
{
	// 방의 상태가  waiting이 아니면 노티를 보내면 안됨
	if (m_currentRoomState != ROOM_STATE::WAITING)
		return;
	
	PacketGameBetCounterNtf pkt;
	pkt._countTime = ServerConfig::bettingTime;
	pkt.minBet = m_pServerConfig->minBet;
	pkt.maxBet = m_pServerConfig->maxBet;

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

// 돈 없는 유저도 쪼까냄
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

		if (user->GetCurMoney() < m_pServerConfig->minBet || user->GetCurMoney() > m_pServerConfig->maxBet)
		{
			LeaveRoom(user);
			//user = nullptr; // 아니.. 이 아래에서 이러면 바로 에러가 나지 ㅠㅠ 내가 한듯. continue 추가함
			continue;
		}

		user->SetGameState(GAME_STATE::BETTING);
	}
}

COMMON::DealerInfo Room::GetDealerInfo()
{
	COMMON::DealerInfo dInfo;
	
	for (int i = 0; i < 7; ++i)
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
		Logger::GetInstance()->Log(Logger::Level::ERROR_NORMAL, L"베팅상태가 아닌데.. 베팅함.", 20);
		return ERROR_CODE::ROOM_GAME_NOT_IN_PROPER_STATE;
	}

	// 서버 설정 돈 보다 작다면..
	if (betMoney < m_pServerConfig->minBet)
		betMoney = m_pServerConfig->minBet;

	// 유저의 돈을 갈취한 뒤..
	auto ret = user->ApplyBet(betMoney);
	if (ret != ERROR_CODE::NONE)
		return ret;
	// DB에 돈 수정해서 보냄.
	m_pDBmanager->SubmitUserEarnMoney(user, -betMoney);

	// 표현할 수 있게 베팅 정보를 노티한다.
	NotifyBetDone(sessionIndex, betMoney);

	// 베팅이 들어온 시점에서 방 안의 유저가 모두 베팅완료라면 게임 시작
	bool isAllBetReadyFlag = true;
	for (auto& user : m_userList)
	{
		if (user == nullptr)
		{
			continue;
		}

		if (user->GetGameState() != GAME_STATE::BET_DONE)
		{
			isAllBetReadyFlag = false;
		}
	}
	if (isAllBetReadyFlag)
	{
		NotifyStartGame();
	}

	return ERROR_CODE::NONE;
}

User* Room::GetCurrentBettingUser()
{
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		User* user = m_userList[i];
		if (user == nullptr)
			continue;

		if (user->GetGameState() == GAME_STATE::ACTIONING)
		{				
			return user;
		}
	}

	return nullptr;
}

void Room::ForceNextTurn(int seat, int hand)
{
	if (m_userList[seat] == nullptr)
	{
		return;
	}

	m_userList[seat]->SetGameState(GAME_STATE::ACTION_DONE);
	m_userList[seat]->SetHandState(hand, COMMON::HandInfo::HandState::STAND);
}

void Room::ForceBetting()
{
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr)
			continue;

		ApplyBet(m_userList[i]->GetSessionIndex(), m_pServerConfig->minBet);
	}
}

ERROR_CODE Room::ApplyChoice(int sessionIndex, ChoiceKind choice)
{
	auto user = GetUserBySessionIndex(sessionIndex);

	switch (choice)
	{
	case ChoiceKind::STAND :
	{
		user->SetHandState(user->GetCurHand(), COMMON::HandInfo::HandState::STAND);
		user->SwitchHandIfSplitExist();
	}
	break;

	case ChoiceKind::HIT:
	{
		user->SetHand(user->GetCurHand(), m_dealer.Draw());

		auto sum = user->GetCardSum(user->GetCurHand());

		if (std::get<0>(sum) > 21)
		{
			user->SetHandState(user->GetCurHand(), COMMON::HandInfo::HandState::BURST);
			// user->SwitchHandIfSplitExist(); // 이 부분은 Game Choice Ntf를 보낸 다음에 호출해야 할 것 같음.
		}
		else if (std::get<0>(sum) == 21 || std::get<1>(sum) == 21)
		{
			user->SetHandState(user->GetCurHand(), COMMON::HandInfo::HandState::STAND);
			// user->SwitchHandIfSplitExist();
		}
		else
		{
			//TODO:: 21보다 작으면 해야할 처리가 있을까?
		}
	}
	break;

	case ChoiceKind::DOUBLE_DOWN:
	{
		if (!user->DoubleDown())
			return ERROR_CODE::ROOM_GAME_NOT_ENOUGH_MONEY;

		// DB에 돈 더 낼것 보냄.
		m_pDBmanager->SubmitUserEarnMoney(user, -(user->GetBetMoney()));

		user->SetHand(user->GetCurHand(), m_dealer.Draw());

		auto sum = user->GetCardSum(user->GetCurHand());

		if (std::get<0>(sum) > 21)
		{
			user->SetHandState(user->GetCurHand(), COMMON::HandInfo::HandState::BURST);
		}
		else
		{
			user->SetHandState(user->GetCurHand(), COMMON::HandInfo::HandState::STAND);
		}
		//바로 턴을 종료함.
		// user->SwitchHandIfSplitExist();

	}
	break;

	case ChoiceKind::SPLIT:
	{
		if (user->IsSplit())
			return ERROR_CODE::ROOM_GAME_INVALID_PLAY_ALREADY_SPLIT;
		
		Logger::GetInstance()->Logf(Logger::Level::INFO, L"Splite! user:%s", user->GetName().c_str());
		user->Split();
		m_pDBmanager->SubmitUserEarnMoney(user, -(user->GetBetMoney()));
	}
	break;
	
	default:
		return ERROR_CODE::ROOM_GAME_INVALID_CHOICE;
	};

	return ERROR_CODE::NONE;
}

std::tuple<int, int> Room::GetNextPlayerSeatAndHand()
{
	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		User* user = m_userList[i];
		if (user == nullptr)
			continue;

		if (user->GetGameState() != GAME_STATE::ACTION_WAITING && user->GetGameState() != GAME_STATE::ACTIONING)
			continue;

		for (int hand = 0; hand < MAX_HAND; ++hand)
		{
			if (hand != user->GetCurHand())
				continue;

			if (user->GetHand(hand)._handState != COMMON::HandInfo::HandState::CURRENT)
				continue;
			
			user->SetGameState(GAME_STATE::ACTIONING);
			return std::make_tuple(i, hand);
		}
	}

	return std::tuple<int, int>(-1, -1);
}

void Room::NotifyStartGame()
{
	// 2초 뒤에 (카드 나눠주는게 끝나면) 첫 번째 유저부터 게임 시작
	m_currentRoomState = ROOM_STATE::HANDOUT;
	m_lastActionTime = duration_cast< milliseconds >(
		steady_clock::now().time_since_epoch()
		).count();

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
			pkt._startSlotNum = m_userList[i]->GetCurSeat();

		}

		m_userList[i]->SetGameState(GAME_STATE::ACTION_WAITING);
	}

	// 상태를 다 바꿨으면 실제로 카드를 나눠주기 시작한다.
	m_dealer.Init(this);
	
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
	if (m_currentRoomState == ROOM_STATE::HANDOUT)
		m_currentRoomState = ROOM_STATE::INGAME;

	PacketGameChangeTurnNtf pkt;
	
	std::tuple<int, int> SeatNhand = GetNextPlayerSeatAndHand();
	
	if (std::get<0>(SeatNhand) == -1 || std::get<1>(SeatNhand) == -1)
	{
		EndOfGame();
		return;
	}

	pkt._slotNum = std::get<0>(SeatNhand);
	pkt._handNum = std::get<1>(SeatNhand);
	pkt._waitingTime = ServerConfig::waitingTime;
	
	m_lastActionTime = duration_cast< milliseconds >(
		steady_clock::now().time_since_epoch()
		).count();

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;
		
		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::GAME_CHANGE_TURN_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}
}

void Room::NotifyGameChoice(int sessionIndex, ChoiceKind choice)
{
	PacketGameChoiceNtf pkt;
	auto user = GetUserBySessionIndex(sessionIndex);

	pkt._slotNum = user->GetCurSeat();
	pkt._choice = choice;

	if (choice == ChoiceKind::HIT || choice == ChoiceKind::DOUBLE_DOWN)
	{
		pkt._recvCard = user->GetLastCard(user->GetCurHand());
	}

	pkt._handNum = user->GetCurHand();
	pkt._betMoney = user->GetBetMoney();
	pkt._currentMoney = user->GetTotalMoney();
	pkt._waitingTime = ServerConfig::waitingTime;

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;
		
		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::GAME_CHOICE_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}

	if (user->GetHand(user->GetCurHand())._handState != HandInfo::HandState::CURRENT)
	{
		user->SwitchHandIfSplitExist();
	}

	m_lastActionTime = duration_cast< milliseconds >(
		steady_clock::now().time_since_epoch()
		).count();
}

void Room::EndOfGame()
{
	using namespace COMMON;
	
	PacketGameDealerResultNtf pkt;

	while (m_dealer.GetCardSum() < 17)
	{
		m_dealer.SetHand(m_dealer.Draw());
	}

	if (m_dealer.GetCardNum() == 7)
	{
		m_dealer.SetHandState(HandInfo::HandState::SEVENCARD);
	}
	else if (m_dealer.GetCardSum() > 21)
	{
		m_dealer.SetHandState(HandInfo::HandState::BURST);
	}
	else
	{
		m_dealer.SetHandState(HandInfo::HandState::STAND);
	}

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		auto user = m_userList[i];
		if (user == nullptr) continue;

		int earnMoney = CalculateEarnMoney(&m_dealer, m_userList[i]);

		Logger::GetInstance()->Logf(Logger::Level::INFO, L"%s : resut : curBetMoney:%d, total EarnMoney:%d", user->GetName().c_str(),user->GetBetMoney() ,earnMoney);

		user->ApplyEarnMoney(earnMoney);
		m_pDBmanager->SubmitUserEarnMoney(user, earnMoney);

		pkt._currentMoney[i] = m_userList[i]->GetCurMoney();
		pkt._earnMoney[i] = earnMoney;

		if (earnMoney > user->GetBetMoney())
		{
			pkt._winYeobu[i] = COMMON::PacketGameDealerResultNtf::WIN_YEOBU::WIN;
			Logger::GetInstance()->Logf(Logger::Level::INFO, L"%s : resut : 이겼음!", user->GetName().c_str());
		}
		else if(earnMoney == user->GetBetMoney()) 
			pkt._winYeobu[i] = COMMON::PacketGameDealerResultNtf::WIN_YEOBU::PUSH;
		else if(earnMoney == 0)
			pkt._winYeobu[i] = COMMON::PacketGameDealerResultNtf::WIN_YEOBU::LOSE;
	}

	for (int i = 1; i < 7; ++i)
	{
		pkt._dealerResult._openedCardList[i - 1] = m_dealer.GetCard(i);
	}

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr) continue;		

		// Res 보냄
		PacketInfo sendPacket;
		sendPacket.SessionIndex = m_userList[i]->GetSessionIndex();
		sendPacket.PacketId = PACKET_ID::GAME_DEALER_RESULT_NTF;
		sendPacket.pRefData = (char *)&pkt;
		sendPacket.PacketBodySize = sizeof(pkt);
		m_pSendPacketQue->PushBack(sendPacket);
	}

	ResetForNextGame();
}

void Room::ResetForNextGame()
{
	// x초 뒤에 시작할 준비
	m_waitForRestart = m_dealer.GetCardNum() * 1.0 + 2;

	m_dealer.Clear();

	for (int i = 0; i < MAX_USERCOUNT_PER_ROOM; ++i)
	{
		if (m_userList[i] == nullptr ) continue;

		m_userList[i]->ResetForNextGame();
	}

	m_currentRoomState = ROOM_STATE::CALCULATE;

	m_lastActionTime = m_lastActionTime = duration_cast< milliseconds >(
		steady_clock::now().time_since_epoch()
		).count();
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

		//if (m_userList[i]->CheckUserWithSessionIndex(sessionIndex))
		//	continue;

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

int Room::CalculateEarnMoney(Dealer * pDealer, User * pUser)
{
	int earnMoney = 0; // 계산 결과 번 돈

	for (int hand = 0; hand < MAX_HAND; ++hand)
	{
		if (pUser->GetCurHand() < hand) 
			continue; //이 방어 코드 왜있는지 모르겠음 ㅠ
		int handEarn = CalculateResultByHand(pDealer->GetHand(), pUser->GetHand(hand), pUser->GetBetMoney()); 
		Logger::GetInstance()->Logf(Logger::Level::INFO, L"%s: hand money result :%d", pUser->GetName().c_str(), handEarn);

		earnMoney += handEarn;
	}
	
	return earnMoney;
}

int Room::CalculateResultByHand(HandInfo dHand, HandInfo uHand, int betMoney)
{
	if (uHand._handState == HandInfo::HandState::CURRENT) return 0;
	float blackjack_bonus = 1.;
	float double_down_bounus = 1.;
	if (uHand._handState == HandInfo::HandState::BLACKJACK)
	{
		blackjack_bonus = 1.5; // 블랙잭이면 1.5배를 줌!
	}

	//더블다운이면 거기에 두배를 줌!
	if (uHand._isDoubledown == true)
	{
		double_down_bounus = 2.;
	}
	auto sum = uHand.GetScore();

	// 만약 유저가 버스트 했으면 무조건 돈을 잃음!
	if (uHand._handState == HandInfo::HandState::BURST)
	{
		return 0;
	}

	// 딜러의 패가 더 높다면.. 돈을 잃음!
	else if (dHand._handState > uHand._handState)
	{
		return 0;
	}

	// 유저 패가 더 높다면.. 돈을 땀!
	else if (dHand._handState < uHand._handState)
	{
		return betMoney * blackjack_bonus * double_down_bounus * 2; // *2를 해줘야 베팅했던 돈까지 돌려받는다.
	}
	// 패가 같은 등급이면
	else
	{
		// 스탠드이면 딸 수도 있지만 아니면 비긴걸로
		if (uHand._handState == HandInfo::HandState::STAND)
		{
			int dealerSum = dHand.GetBetterScore();
			//Ace가 섞인 경우 더 좋은 숫자를 선택
			
			int useSum = uHand.GetBetterScore();
			//int useSum = 0;
			//if (std::get<0>(sum) == std::get<1>(sum))
			//	useSum = std::get<0>(sum);
			//else if (std::get<1>(sum) > 21) // Ace를 11로 계산하면 안되는 경우
			//	useSum = std::get<0>(sum);
			//else
			//	useSum = std::get<1>(sum);

			if (useSum > dealerSum) // 이겼을 때.
			{
				return betMoney * double_down_bounus * 2;
			}
			else if (useSum < dealerSum) // 졌을때
			{
				return 0;
			}
			else // 비겼을때
			{
				return betMoney * double_down_bounus;
			}
		}
		else // stand가 아닌데 같은 등급일때
		{
			return betMoney * double_down_bounus;
		}
	}

	//여기에 올일이 없어야 한다. 
	assert("Not_Expected_exception" == nullptr);
	return 0;
}
