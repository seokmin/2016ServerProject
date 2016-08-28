#include "stdafx.h"
#include "Common.h"
#include "PacketProcess.h"
#include "RoomManager.h"

ERROR_CODE PacketProcess::GameBet(PacketInfo packetInfo)
{
	auto reqPkt = (PacketGameBetReq*)packetInfo.pRefData;
	
	auto& room = m_pRefRoomMgr->GetRoomBySessionIndex(packetInfo.SessionIndex);

	auto ret = room->ApplyBet(packetInfo.SessionIndex, reqPkt->_betMoneyReq);
	if (ret != ERROR_CODE::NONE)
		return ret;




}
