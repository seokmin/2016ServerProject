#pragma once

#include "DBmanager.h"

class UserManager;
class RoomManager;
struct DBResult;
class PacketQueue;

class DBProcess
{
	typedef ERROR_CODE (DBProcess::*DBFunc)(DBResult);
	DBFunc DBFuncArray[(int)JOB_TYPE::MAX];

public:
	DBProcess() {};
	~DBProcess() {};

	void Init(UserManager* userMgr, RoomManager* roomMgr, PacketQueue* m_pSendPacketQue, DBmanager* m_pDBMgr);
	
	void Process(DBResult rslt);


private:
	ERROR_CODE SubmitStateProcess(DBResult ret);
	ERROR_CODE GetUserByAuthProcess(DBResult ret);
	ERROR_CODE CalculateMoneyResProcess(DBResult ret);
	ERROR_CODE ClearUserAuthToken(DBResult ret);

private:
	UserManager*			m_pUserMgr;
	RoomManager*			m_pRoomMgr;
	PacketQueue*			m_pSendPacketQue;
	DBmanager*				m_pDBMgr;
};
