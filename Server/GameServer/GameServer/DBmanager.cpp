#include "stdafx.h"
#include "DBmanager.h"
#include "MysqlManager.h"

DBmanager::DBmanager()
{
}


DBmanager::~DBmanager()
{
}

COMMON::ERROR_CODE DBmanager::Init()
{
	MySQLMangager mysql;
	SQLWCHAR result[100];
	auto ret = mysql.sqlconn();
	ret = mysql.sqlexec(L"USE jackblack;", result);
	mysql.sqldisconn();
	if (!MYSQLSUCCESS(ret))
	{
		return COMMON::ERROR_CODE::DB_ERROR;
	}
	return COMMON::ERROR_CODE::NONE;
}

COMMON::ERROR_CODE DBmanager::SubmitState(int max, int count, ServerConfig* pServerConfig)
{
	//	"SELECT name, ip, port, r, g, b, minBet, maxBet FROM channel"
	SQLWCHAR query[200] = L"";
	SQLWCHAR result[200];
	MySQLMangager mysql;
	
	swprintf_s(query, 200,
		L"CALL submit_server_status('%ls', '%ls', '%d', %hd, %d, %d, %d, %d, %d, %d);\n",
		pServerConfig->SERVERNAME, pServerConfig->IP, pServerConfig->Port, 
		100, 150, 150, //rgb
		pServerConfig->minBet, pServerConfig->maxBet, 
		count, max // user count
	);
	//wprintf_s(query);

	auto ret = mysql.sqlconn();
	ret = mysql.sqlexec(query, result);
	mysql.sqldisconn();
	if (!MYSQLSUCCESS(ret))
	{
		return COMMON::ERROR_CODE::DB_ERROR;
	}
	
	//wprintf_s(L"Serverid:%s\n", result); //result
	return COMMON::ERROR_CODE::NONE;
}
