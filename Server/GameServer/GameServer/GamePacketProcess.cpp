#include "stdafx.h"
#include "Common.h"
#include "PacketProcess.h"
#include "RoomManager.h"
#include "UserManager.h"

ERROR_CODE PacketProcess::GameBet(PacketInfo packetInfo)
{
	auto reqPkt = (PacketGameBetReq*)packetInfo.pRefData;
	
	auto& room = m_pRefRoomMgr->GetRoomBySessionIndex(packetInfo.SessionIndex);

	auto ret = room->ApplyBet(packetInfo.SessionIndex, reqPkt->_betMoneyReq);
	if (ret != ERROR_CODE::NONE)
		return ret;

	return ERROR_CODE::NONE;
}

ERROR_CODE PacketProcess::GameChoice(PacketInfo packetInfo)
{
	auto reqPkt = (PacketGameChoiceReq*)packetInfo.pRefData;
	
	auto& room = m_pRefRoomMgr->GetRoomBySessionIndex(packetInfo.SessionIndex);
	std::tuple<int, int> prevSeatNhand = room->GetNextPlayerSeatAndHand();
	
	auto ret = room->ApplyChoice(packetInfo.SessionIndex, reqPkt->_choice);
	if (ret != ERROR_CODE::NONE)
	{
		room->NotifyChangeTurn();
		return ret;
	}

	room->NotifyGameChoice(packetInfo.SessionIndex, reqPkt->_choice);

	// 핸드가 다음 핸드로 플레이 될 때만 노티를 보냄.
	if(prevSeatNhand != room->GetNextPlayerSeatAndHand())
		room->NotifyChangeTurn();
	
	
	return ERROR_CODE::NONE;
}

