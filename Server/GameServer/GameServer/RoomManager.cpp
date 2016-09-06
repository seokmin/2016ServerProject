#include "stdafx.h"
#include "RoomManager.h"
#include "UserManager.h"
#include "PacketQueue.h"
#include "DBmanager.h"

void RoomManager::Init(UserManager* pUserMan, DBmanager* pDBman, PacketQueue* sendPacketQue, ServerConfig* serverConfig)
{
	m_pRefUserManager = pUserMan;
	m_pServerConfig = serverConfig;

	for (int i = 0; i < ServerConfig::MAX_ROOMCOUNT; i++)
	{
		auto room = std::make_shared<Room>(i);
		room->Init(sendPacketQue, pDBman, serverConfig);
		m_roomList.push_back(room);
	}
}

ERROR_CODE RoomManager::EnterUser(int sessionIndex)
{
	std::shared_ptr<Room> targetRoom = GetAvailableRoom();
	if (targetRoom == nullptr)
	{
		printf_s("유저(%d)가 들어갈 수 있는 방이 없습니다. \n", sessionIndex);
		return ERROR_CODE::ROOM_ENTER_CHANNEL_FULL;
	}

	auto targetUser = m_pRefUserManager->GetUserBySessionIndex(sessionIndex);
	if (targetUser == nullptr)
	{
		printf_s("해당 세션의 유저(%d)가 없어서 방에 들어갈 수 없습니다. \n", sessionIndex);
		return ERROR_CODE::ROOM_ENTER_INVALID_SESSION_INDEX;
	}

	if (!targetRoom->EnterUser(targetUser.get()))
	{
		return ERROR_CODE::ROOM_ENTER_ROOM_FULL;
	}

	return ERROR_CODE::NONE;
}

std::shared_ptr<Room> RoomManager::GetRoomBySessionIndex(int sessionIdx)
{
	for (auto& room : m_roomList)
	{
		if (room->GetCurrentUserCount() == 0)
			continue;

		if (room->CheckUserExist(sessionIdx))
			return room;
	}
	return nullptr;
}

void RoomManager::RunPostTimeAction()
{
	for (auto& room : m_roomList)
	{
		// 유저가 0이면 그냥 나감..
		if (room->GetCurrentUserCount() == 0)
			continue;

		switch (room->GetCurrentRoomState())
		{
		case ROOM_STATE::NONE :
		{
			// 여기까지 왔는데 상태가 NONE일 수는 없음.
			WCHAR errStr[100];
			wsprintf(errStr, L"룸(%d)의 상태가 NONE인 거에 접근했음!", room->GetRoomId());
			Logger::GetInstance()->Log(Logger::INFO, errStr, 100);
			break;
		}
		break;

		case ROOM_STATE::WAITING :
		{
			// 게임을 시작하려고 기다리는 중이면..
			// 그리고 15초가 넘게 이 상태였으면
			// "자 이제 게임을 시작하지"

			auto nowTime = duration_cast< milliseconds >(
				steady_clock::now().time_since_epoch()
				).count();

			if (nowTime - room->GetLastActionTime() > ServerConfig::bettingTime * 1000)
			{
				room->ForceBetting();
				//room->NotifyStartGame(); // 이 부분이 노티파이가 있어서 뺌..
			}
		}
		break;

		case ROOM_STATE::HANDOUT :
		{
			// 카드 나눠주고 2초 지난 뒤 자동으로 항상 시작
			auto nowTime = duration_cast< milliseconds >(
				steady_clock::now().time_since_epoch()
				).count();

			if (nowTime - room->GetLastActionTime() > 2 * 1000)
			{
				// 사실상 이때부터 게임 시작.
				room->NotifyChangeTurn();
			}
		}
		break;

		case ROOM_STATE::INGAME :
		{
			// 이 경우는 행동을 결정할 시간이 지나도 상태가 그대로일 때..

			auto nowTime = duration_cast< milliseconds >(
				steady_clock::now().time_since_epoch()
				).count();

			if (nowTime - room->GetLastActionTime() > ServerConfig::waitingTime * 1000)
			{
				auto user = room->GetCurrentBettingUser();
				if (user == nullptr)
				{
					// 게임중에 10초가 지났는데.. 베팅중인 유저가 없으면 버그!
					WCHAR errStr[100];
					wsprintf(errStr, L"룸(%d)의 상태가 INGAME, 시간이 지났는데 베팅유저가 없음!!", room->GetRoomId());
					Logger::GetInstance()->Log(Logger::INFO, errStr, 100);
					
					room->EndOfGame();
					break;
				}
				room->ForceNextTurn(user->GetCurSeat(), user->GetCurHand());
				room->NotifyGameChoice(user->GetSessionIndex(), COMMON::ChoiceKind::STAND);
				room->NotifyChangeTurn();
			}
		}
		break;

		case ROOM_STATE::CALCULATE :
		{
			// 애니메이션 끝난 뒤 게임 재시작

			auto nowTime = duration_cast< milliseconds >(
				steady_clock::now().time_since_epoch()
				).count();

			if (nowTime - room->GetLastActionTime() > room->GetWaitingForRestart() * 1000)
			{
				room->SetRoomStateToWaiting(); // 여기서 돈 없는 유저도 쫒아낼거임.
				room->NotifyStartBettingTimer();
			}
		}
		break;

		}
	}
}

std::shared_ptr<Room> RoomManager::GetAvailableRoom()
{
	int maxUserNum = ServerConfig::MAX_USERCOUNT_PER_ROOM;
	auto iter = std::find_if(std::begin(m_roomList), std::end(m_roomList), [maxUserNum](auto pRoom) {return pRoom->GetCurrentUserCount() > 0 && pRoom->GetCurrentUserCount() < maxUserNum; });
	if (iter != std::end(m_roomList))
		return *iter;

	iter = std::find_if(std::begin(m_roomList), std::end(m_roomList), [](auto pRoom) {return pRoom->GetCurrentUserCount() == 0; });
	if (iter != std::end(m_roomList))
		return *iter;

	return nullptr;
}

COMMON::ERROR_CODE RoomManager::LeavUserFromRoom(int roomid, User * pUser)
{
	return m_roomList[roomid]->LeaveRoom(pUser);
}

std::shared_ptr<Room> RoomManager::GetRoomById(int id)
{
	return m_roomList[id];
}
