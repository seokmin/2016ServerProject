#pragma once

#include "DBmanager.h"

class UserManager;
class RoomManager;
struct DBResult;
class PacketQueue;

class DBProcess
{
	typedef void(DBProcess::*DBFunc)(DBResult);
	DBFunc DBFuncArray[(int)JOB_TYPE::MAX];

public:
	DBProcess() {};
	~DBProcess() {};

	void Init(UserManager* userMgr, RoomManager* roomMgr, PacketQueue* m_pSendPacketQue, DBmanager* m_pDBMgr);
	
	void Process(DBResult rslt);


private:
	UserManager*			m_pUserMgr;
	RoomManager*			m_pRoomMgr;
	PacketQueue*			m_pSendPacketQue;
	DBmanager*				m_pDBMgr;
};
