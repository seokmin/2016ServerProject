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

	DBFuncArray[(int)JOB_TYPE::NONE] = nullptr;
	DBFuncArray[(int)JOB_TYPE::SUBMIT_STATE] = &DBProcess::SubmitStateProcess;
	DBFuncArray[(int)JOB_TYPE::GET_USER_INFO_BY_AUTH] = &DBProcess::GetUserByAuthProcess;
	DBFuncArray[(int)JOB_TYPE::CALCULATE_MONEY] = &DBProcess::CalculateMoneyResProcess;
}

void DBProcess::Process(DBResult rslt)
{
	auto resultType = rslt._type;

	if (DBFuncArray[(int)resultType] == nullptr)
	{
		Logger::GetInstance()->Logf( Logger::Level::ERROR_FATAL ,L"존재하지 않는 ID의 DB결과입니다. 위치 : DB::Process");
		return;
	}

	(this->*DBFuncArray[(int)resultType])(rslt);
}

ERROR_CODE DBProcess::SubmitStateProcess(DBResult rslt)
{
	if (rslt._retCode != SQL_SUCCESS)
	{
		Logger::GetInstance()->Logf(Logger::ERROR_FATAL, L"=========[DB : FATAL] Faild to Write DB :: Server Id = %s %s ", rslt._result1, rslt._result2);
	}
	return ERROR_CODE();
}

ERROR_CODE DBProcess::GetUserByAuthProcess(DBResult rslt)
{
	if (rslt._retCode != SQL_SUCCESS)
	{
		Logger::GetInstance()->Logf(Logger::WARNING, L"[DB : FAIL TO EXECUTE] Error = %s", rslt._retCode);

		DBJob job;
		int inputIndex = rslt._sessionIndex % ServerConfig::numberOfDBThread;
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
		return result;
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
	return ERROR_CODE();
}

ERROR_CODE DBProcess::CalculateMoneyResProcess(DBResult rslt)
{
	Logger::GetInstance()->Logf(Logger::INFO, L"[Submit DB(User money): Success]name:%s , money : %s", rslt._result1, rslt._result2);
	return ERROR_CODE();
}
