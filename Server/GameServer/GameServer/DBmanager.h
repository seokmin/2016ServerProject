#pragma once
#include "ServerConfig.h"
#include <windows.h>
#include <queue>
#include <sqlext.h>
#include <mutex>

class Logger;
class MySQLMangager;


enum class JOB_TYPE
{
	NONE = 0,
	SUBMIT_STATE = 1,
	GET_USER_INFO_BY_AUTH = 2,

	MAX = 255,
};

struct DBJob
{
	JOB_TYPE	_type = JOB_TYPE::NONE;
	SQLWCHAR	_query[200];
	int			_nResult = 0;
	int			_sessionIndex = -1;

	void Reset()
	{
		_type = JOB_TYPE::NONE;
		_nResult = 0;
		_sessionIndex = -1;
		memset(&_query, 0, sizeof(_query));
	}
};

struct DBResult
{
	RETCODE	_retCode = SQL_FALSE;
	JOB_TYPE _type = JOB_TYPE::NONE;
	SQLWCHAR _result1[100];
	SQLWCHAR _result2[100];
	SQLWCHAR _result3[100];
	SQLWCHAR _result4[100];
	SQLWCHAR _query[200];
	int		_sessionIndex = -1;

	void Reset()
	{
		_retCode = SQL_FALSE;
		_type = JOB_TYPE::NONE;
		_sessionIndex = -1;
		memset(&_result1, 0, sizeof(_result1));
		memset(&_result2, 0, sizeof(_result2));
		memset(&_result3, 0, sizeof(_result3));
		memset(&_result4, 0, sizeof(_result4));
		memset(&_query, 0, sizeof(_query));
	}
};

class DBmanager
{
public:
	DBmanager() {};
	virtual ~DBmanager();
	COMMON::ERROR_CODE Init(int numberOfDBThread);
	void SubmitState(int max, int count, ServerConfig* serverConfig);

	long GetThreadIndex();
	void DBThreadWorker();

	bool		DBResultEmpty() { return m_jobResultQ.empty(); };
	DBResult	FrontDBResult();
	void		PopDBResult();

	void		PushDBJob(DBJob job, int pushIndex);

private:
	HANDLE					hDBEvent[4];
	std::deque<DBJob>		m_jobQ[4];
	std::deque<DBResult>	m_jobResultQ;
	MySQLMangager*			m_sqlMgrPool[4];
	DBJob					m_jobPool[4];
	DBResult				m_resultPool[4];

	std::mutex				m_mutex;
	std::mutex				m_mutex_result;

};

