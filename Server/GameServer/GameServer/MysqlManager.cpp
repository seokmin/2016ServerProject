#include "stdafx.h"
#include "MysqlManager.h"
//#include "Logger.h"

MySQLMangager::MySQLMangager()
{
	wcscpy_s(chr_ds_name, SQL_MAX_DSN_LENGTH, (SQLWCHAR *)L"mysql_seokmin_for_jb"); // odbc name
	auto tmp = hstmt;
}


MySQLMangager::~MySQLMangager()
{
}

// Allocate environment handle and connection handle, connect to data source, and allocate statement handle.
RETCODE MySQLMangager::sqlconn() {
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv, &hdbc);
	//rc = SQLConnect(hdbc, chr_ds_name, SQL_NTS, ConnID, sizeof(ConnID), ConnPW, sizeof(ConnPW));
	rc = SQLConnect(hdbc, chr_ds_name, SQL_NTS, NULL, 0, NULL, 0);
	// Deallocate handles, display error message, and exit.
	if (!MYSQLSUCCESS(rc)) {
		SQLWCHAR	SQLState[100];
		SQLWCHAR	MessageText[100];
		SQLINTEGER	NativeErrorPtr;
		SQLSMALLINT TextLength;
		SQLGetDiagRec(
			SQL_HANDLE_DBC,
			hdbc,
			1,
			SQLState,
			&NativeErrorPtr,
			MessageText,
			100,
			&TextLength);

		SQLFreeConnect(henv);
		SQLFreeEnv(henv);
		SQLFreeConnect(hdbc);
		if (hstmt)
			error_out();
		return rc;
	}

	return rc = SQLAllocStmt(hdbc, &hstmt);
}

// Execute SQL command with SQLExecDirect() ODBC API.
RETCODE MySQLMangager::sqlexec(SQLWCHAR * query, int n_args, ...) {
	Logger::GetInstance()->Logf(Logger::Level::INFO, L"Query send : %s", query);
	rc = SQLExecDirect(hstmt, query, SQL_NTS);
	if (!MYSQLSUCCESS(rc)) {  //Error
		error_out();
		// Deallocate handles and disconnect.
		SQLFreeStmt(hstmt, SQL_DROP);
		SQLDisconnect(hdbc);
		SQLFreeConnect(hdbc);
		SQLFreeEnv(henv);
		return rc;
	}
	else {
		va_list ap;
		va_start(ap, n_args);

		for (rc = SQLFetch(hstmt); rc == SQL_SUCCESS; rc = SQLFetch(hstmt)) {
			for (int i = 0; i < n_args; ++i)
			{
				SQLWCHAR* rslt = va_arg(ap, SQLWCHAR*);
				SQLGetData(hstmt, i+1, SQL_C_WCHAR, rslt, MAX_DATA, &cbData);
				Logger::GetInstance()->Logf(Logger::Level::INFO, L"%ls", rslt);
			}
			//swprintf_s(result, cbData, L"%ls", szData);
		}
	}
	return SQL_SUCCESS;
}

// Free the statement handle, disconnect, free the connection handle, and free the environment handle.
void MySQLMangager::sqldisconn() {
	SQLFreeStmt(hstmt, SQL_DROP);
	SQLDisconnect(hdbc);
	SQLFreeConnect(hdbc);
	SQLFreeEnv(henv);
}

// Display error message in a message box that has an OK button.
void MySQLMangager::error_out() {
	SQLWCHAR szSQLSTATE[10];
	SDWORD nErr;
	SQLWCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1];
	SWORD cbmsg;

	while (SQLError(0, 0, hstmt, szSQLSTATE, &nErr, msg, sizeof(msg), &cbmsg) == SQL_SUCCESS) {
		Logger::GetInstance()->Logf(Logger::Level::WARNING, L"Error:\nSQLSTATE=%ls, Native error=%ld, msg='%ls'", szSQLSTATE, nErr, msg);
		
		//wprintf_s(L"Error:\nSQLSTATE=%ls, Native error=%ld, msg='%ls'", szSQLSTATE, nErr, msg);

		//swprintf_s(szData, sizeof(szData), L"Error:\nSQLSTATE=%ls, Native error=%ld, msg='%ls'", szSQLSTATE, nErr, msg);
		//MessageBox(NULL, szData, L"ODBC Error", MB_OK);
	}
}