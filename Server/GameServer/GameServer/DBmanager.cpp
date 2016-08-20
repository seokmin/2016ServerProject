#include "stdafx.h"
#include "DBmanager.h"
#include "MysqlManager.h"

DBmanager::DBmanager()
{
}


DBmanager::~DBmanager()
{
}

COMMON::ERROR_CODE DBmanager::SubmitState(int max, int count)
{
	/*멀티스레드로 바꾸고 싶으면 이 부분을 고치시오*/
	SQLWCHAR query[100];
	SQLWCHAR result[100];
	MySQLMangager mysql;
	auto ret = mysql.sqlconn();
	ret = mysql.sqlexec(query, result);
	mysql.sqldisconn();
	if (!MYSQLSUCCESS(ret))
	{
		return COMMON::ERROR_CODE::DB_ERROR;
	}

	return COMMON::ERROR_CODE::NONE;
}
