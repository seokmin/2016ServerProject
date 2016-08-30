#include "stdafx.h"
#include "DBProcess.h"
#include "Logger.h"
#include "UserManager.h"
#include "User.h"
#include "RoomManager.h"
#include "Room.h"
#include "Common.h"
#include "PacketQueue.h"

void DBProcess::Init(UserManager * userMgr, RoomManager * roomMgr, PacketQueue* packetQ, DBmanager* dbMgr)
{
	m_pUserMgr = userMgr;
	m_pRoomMgr = roomMgr;
	m_pSendPacketQue = packetQ;
	m_pDBMgr = dbMgr;
}

void DBProcess::Process(DBResult rslt)
{
	switch (rslt._type)
	{
	case JOB_TYPE::SUBMIT_STATE:
	{
		Logger::GetInstance()->Logf(Logger::INFO, L"[DB : GOOD] Server Id = %s %s ", rslt._result1, rslt._result2);
	}
	break;

	case JOB_TYPE::GET_USER_INFO_BY_AUTH:
	{
		if (rslt._retCode != SQL_SUCCESS)
		{
			Logger::GetInstance()->Logf(Logger::WARNING, L"[DB : FAIL TO EXECUTE] Error = %s", rslt._retCode);

			DBJob job;
			int inputIndex = rslt._sessionIndex % 4;
			job._type = rslt._type;
			job._nResult = 4;
			job._sessionIndex = rslt._sessionIndex;

			m_pDBMgr->PushDBJob(job, inputIndex);
		}

		std::wstring wAuth(rslt._result4);
		std::string sAuth(wAuth.begin(), wAuth.end());
		auto user = m_pUserMgr->GetUserBySessionId(rslt._sessionIndex);

		// 로그인 로직 시작
		user->Init(sAuth, rslt._result1, _wtoi(rslt._result2), _wtoi(rslt._result3));
		user->SetIoState(IO_STATE::NONE);

		Logger::GetInstance()->Logf(Logger::INFO, L"[LOGIC, DB : SUCCESS] User(%s) Logged In", rslt._result1);
		
		// RES 로직 시작
		PacketRoomEnterRes resPkt;

		ERROR_CODE result = m_pRoomMgr->EnterUser(rslt._sessionIndex);
		if (result != ERROR_CODE::NONE)
		{
			Logger::GetInstance()->Logf(Logger::ERROR_FATAL, L"[LOGIC, DB : FAIL] User(%s) Failed to Enter Room!!! =========", rslt._result1);
			return;
		}

		Logger::GetInstance()->Logf(Logger::INFO, L"[LOGIC, DB : SUCCESS] User(%s) Entered Room(%d)", rslt._result1, user->GetCurRoomIdx());
		
		resPkt._roomNum = user->GetCurRoomIdx();
		
		// Res 보냄
		RecvPacketInfo sendPacket;
		sendPacket.SessionIndex = rslt._sessionIndex;
		sendPacket.PacketId = PACKET_ID::ROOM_ENTER_RES;
		sendPacket.pRefData = (char *)&resPkt;
		sendPacket.PacketBodySize = sizeof(resPkt);
		m_pSendPacketQue->PushBack(sendPacket);
		
		// 나머지 유저들한테 Notify
		m_pRoomMgr->GetRoomBySessionIndex(rslt._sessionIndex)->NotifyEnterUserInfo(rslt._sessionIndex);
	}
	break;

	case JOB_TYPE::NONE:
	{

	}
	break;

	default:
		break;
	}
}
