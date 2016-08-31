#include "stdafx.h"
#include "DBmanager.h"
#include "MysqlManager.h"
#include "Logger.h"
#include "User.h"

__declspec(thread) long g_nThreadIndex = 0;
volatile long g_nThreadSeq = -1;

DBmanager::~DBmanager()
{
	delete[] m_sqlMgrPool;
}

COMMON::ERROR_CODE DBmanager::Init(int numberOfDBThread)
{
	MySQLMangager mysql;
	auto ret = mysql.sqlconn();
	ret = mysql.sqlexec(L"USE jackblack;", 0);
	mysql.sqldisconn();
	if (!MYSQLSUCCESS(ret))
	{
		return COMMON::ERROR_CODE::DB_ERROR;
	}

	// 설정된 값 만큼 DB 처리 쓰레드 생성.
	// 쓰레드별 이벤트 생성
	// SQLMgr 생성
	for (int i = 0; i < numberOfDBThread; ++i)
	{
		m_sqlMgrPool[i] = new MySQLMangager();
		
		hDBEvent[i] = CreateEvent(0, TRUE, FALSE, 0);

		auto dbThread = std::thread(std::bind(&DBmanager::DBThreadWorker, this));
		dbThread.detach();
	}

	return COMMON::ERROR_CODE::NONE;
}

long DBmanager::GetThreadIndex()
{
	if (g_nThreadIndex == 0)
	{
		g_nThreadIndex = InterlockedIncrement(&g_nThreadSeq);
	}

	return g_nThreadIndex;
}

void DBmanager::DBThreadWorker()
{
	DWORD ret;

	int index = GetThreadIndex();

	while (true)
	{
		ret = WaitForSingleObject(hDBEvent[index], INFINITE);
		if (ret == WAIT_FAILED)
		{
			WCHAR levelStr[200];
			wsprintf(levelStr, L"DB 쓰레드(%d)의 죽음을 알리지 말라..", index);
			Logger::GetInstance()->Log(Logger::INFO, levelStr, 200);
			break;
		}

		m_mutex.lock();
		if (m_jobQ[index].empty())
		{
			m_mutex.unlock();
			WCHAR levelStr[200];
			wsprintf(levelStr, L"DB 쓰레드(%d)에서 이벤트를 받았는데 Q에 아무것도 없음.. ", index);
			Logger::GetInstance()->Log(Logger::INFO, levelStr, 200);
			ResetEvent(hDBEvent[index]);
			return;
		}
		m_mutex.unlock();

		m_mutex.lock();
		m_jobPool[index] = m_jobQ[index].front();
		m_jobQ[index].pop_front();
		m_mutex.unlock();

		auto ret = m_sqlMgrPool[index]->sqlconn();
		ret = m_sqlMgrPool[index]->sqlexec(m_jobPool[index]._query, m_jobPool[index]._nResult, m_resultPool[index]._result1, m_resultPool[index]._result2, m_resultPool[index]._result3, m_resultPool[index]._result4);
		m_sqlMgrPool[index]->sqldisconn();

		if (!MYSQLSUCCESS(ret))
		{
			WCHAR levelStr[200];
			wsprintf(levelStr, L"DBT(%d) DB Upsert 결과 실패.. 실패 결과를 큐에 전달함", index);
			Logger::GetInstance()->Log(Logger::INFO, levelStr, 200);
		}

		m_resultPool[index]._retCode = ret;
		m_resultPool[index]._type = m_jobPool[index]._type;
		m_resultPool[index]._sessionIndex = m_jobPool[index]._sessionIndex;
		wcscpy_s(m_resultPool[index]._query, m_jobPool[index]._query);

		m_jobPool[index].Reset();
		m_mutex.lock();
		m_jobResultQ.push_back(m_resultPool[index]);
		m_mutex.unlock();
		m_resultPool[index].Reset();

		ResetEvent(hDBEvent[index]);

		
	}
}

DBResult DBmanager::FrontDBResult()
{
	m_mutex_result.lock();
	if (m_jobResultQ.empty())
	{
		m_mutex_result.unlock();
		return DBResult();
	}
	m_mutex_result.unlock();
	return m_jobResultQ.front();
}

void DBmanager::PopDBResult()
{
	m_mutex_result.lock();
	if (m_jobResultQ.empty())
	{
		m_mutex_result.unlock();
		return;
	}
	m_mutex_result.unlock();
	m_jobResultQ.pop_front();
}

void DBmanager::PushDBJob(DBJob job, int pushIndex)
{
	m_mutex.lock();
	m_jobQ[pushIndex].push_back(job);
	m_mutex.unlock();
	SetEvent(hDBEvent[pushIndex]);
}

void DBmanager::SubmitState(int max, int count, ServerConfig* pServerConfig)
{
	//	"SELECT name, ip, port, r, g, b, minBet, maxBet FROM channel"

	DBJob submitJob;
	
	submitJob._type = JOB_TYPE::SUBMIT_STATE;
	swprintf_s(submitJob._query, 200,
		L"CALL submit_server_status('%ls', '%ls', '%d', %hd, %d, %d, %d, %d, %d, %d);",
		pServerConfig->SERVERNAME, pServerConfig->IP, pServerConfig->Port, 
		100, 150, 150, // rgb
		pServerConfig->minBet, pServerConfig->maxBet, 
		count, max // user count
	);
	submitJob._nResult = 2;
	//wprintf_s(query);

	// 이게 하나씩 늘어나면서 어느 디비쓰레드가 작업할지 정해줌.
	static int dbCount = 0;
	if (dbCount == 4) dbCount = 0;

	m_mutex.lock();
	m_jobQ[dbCount].push_back(submitJob);
	m_mutex.unlock();
	SetEvent(hDBEvent[dbCount++]);
}


void DBmanager::SubmitUserDeltaMoney(User * pUser, int deltaMoney)
{
	DBJob calcMoneyJob;

	SQLWCHAR query[200] = L"";
	wsprintf(calcMoneyJob._query, L"CALL Calc_user_money(\"%s\", %d);", pUser->GetName().c_str(), deltaMoney);
	calcMoneyJob._sessionIndex = pUser->GetSessionIndex();
	calcMoneyJob._type = JOB_TYPE::CALCULATE_MONEY;
	calcMoneyJob._nResult = 2;
	wprintf_s(query);

	Logger::GetInstance()->Logf(Logger::Level::INFO, L"submit money to DB: %d ->%d", pUser->GetTotalMoney(), pUser->GetTotalMoney() + deltaMoney);
	PushDBJob(calcMoneyJob, 0);
}
