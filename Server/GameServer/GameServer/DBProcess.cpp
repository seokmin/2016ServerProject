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
		WCHAR levelStr[200];
		if (rslt._retCode != SQL_SUCCESS)
		{
			// 에러만 띄우고 암것도 안 함.
			wsprintf(levelStr, L"[DB : FAIL TO WRITE] Server Id = %s %s", rslt._result1, rslt._result2);
		}
		else
		{
			wsprintf(levelStr, L"[DB : GOOD] Server Id = %s %s", rslt._result1, rslt._result2);
		}

		Logger::GetInstance()->Log(Logger::INFO, levelStr, 200);
	}
	break;

	case JOB_TYPE::GET_USER_INFO_BY_AUTH:
	{
		if (rslt._retCode != SQL_SUCCESS)
		{
			WCHAR levelStr[200];
			wsprintf(levelStr, L"[DB : FAIL TO EXECUTE] Error = %s", rslt._retCode);
			Logger::GetInstance()->Log(Logger::WARNING, levelStr, 200);

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

		WCHAR logStr[200];
		wsprintf(logStr, L"[LOGIC, DB : SUCCESS] User(%s) Logged In", rslt._result1);
		Logger::GetInstance()->Log(Logger::INFO, logStr, 200);
		
		// RES 로직 시작
		PacketRoomEnterRes resPkt;

		ERROR_CODE result = m_pRoomMgr->EnterUser(rslt._sessionIndex);
		if (result != ERROR_CODE::NONE)
		{
			WCHAR levelStr[200];
			wsprintf(levelStr, L"[LOGIC, DB : FAIL] User(%s) Failed to Enter Room!!! =========", rslt._result1);
			Logger::GetInstance()->Log(Logger::ERROR_FATAL, levelStr, 200);
			return;
		}

		WCHAR levelStr[200];
		wsprintf(levelStr, L"[LOGIC, DB : SUCCESS] User(%s) Entered Room(%d)", rslt._result1, user->GetCurRoomIdx());
		Logger::GetInstance()->Log(Logger::INFO, levelStr, 200);
		
		resPkt._roomNum = user->GetCurRoomIdx();
		
		// Res 보냄
		RecvPacketInfo sendPacket;
		sendPacket.SessionIndex = rslt._sessionIndex;
		sendPacket.PacketId = PACKET_ID::ROOM_ENTER_RES;
		sendPacket.pRefData = (char *)&resPkt;
		sendPacket.PacketBodySize = sizeof(resPkt);
		m_pSendPacketQue->PushBack(sendPacket);
		
		// Notify
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
