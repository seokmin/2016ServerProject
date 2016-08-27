#include "stdafx.h"
#include "Common.h"
#include "PacketProcess.h"
#include "RoomManager.h"

ERROR_CODE PacketProcess::GameBet(PacketInfo packetInfo)
{
	auto reqPkt = (PacketGameBetReq*)packetInfo.pRefData;
	
	auto& room = m_pRefRoomMgr->GetRoomBySessionIndex(packetInfo.SessionIndex);

	room->ApplyBet(packetInfo.SessionIndex, reqPkt->_betMoneyReq);
}
