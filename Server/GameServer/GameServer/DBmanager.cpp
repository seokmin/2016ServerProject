#include "stdafx.h"
#include "DBmanager.h"
#include "MysqlManager.h"
#include <wchar.h>

DBmanager::DBmanager()
{
}


DBmanager::~DBmanager()
{
}

COMMON::ERROR_CODE DBmanager::SubmitState(int max, int count)
{
	/*��Ƽ������� �ٲٰ� ������ �� �κ��� ��ġ�ÿ�*/
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