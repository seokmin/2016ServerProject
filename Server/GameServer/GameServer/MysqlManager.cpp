#include "stdafx.h"
#include "MysqlManager.h"

MySQLMangager::MySQLMangager()
{
	wcscpy_s(chr_ds_name, SQL_MAX_DSN_LENGTH, (SQLWCHAR *)L"mysql_seokmin_for_jb"); // odbc name
}


MySQLMangager::~MySQLMangager()
{
}

// Allocate environment handle and connection handle, connect to data source, and allocate statement handle.
RETCODE MySQLMangager::sqlconn() {
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv, &hdbc);
	SQLWCHAR ConnID[] = L"next";
	SQLWCHAR ConnPW[] = L"1234";
	//rc = SQLConnect(hdbc, chr_ds_name, SQL_NTS, ConnID, sizeof(ConnID), ConnPW, sizeof(ConnPW));
	rc = SQLConnect(hdbc, chr_ds_name, SQL_NTS, NULL, 0, NULL, 0);
	// Deallocate handles, display error message, and exit.
	if (!MYSQLSUCCESS(rc)) {
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
RETCODE MySQLMangager::sqlexec(SQLWCHAR * query, SQLWCHAR * result) {
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
		for (rc = SQLFetch(hstmt); rc == SQL_SUCCESS; rc = SQLFetch(hstmt)) {
			SQLGetData(hstmt, 1, SQL_C_WCHAR, szData, sizeof(szData), &cbData);
			// In this example, the data is sent to the console; SQLBindCol() could be called to bind 
			// individual rows of data and assign for a rowset.
			swprintf_s(result, cbData, L"%ls", szData);
			wprintf(L"%ls\n", szData);
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
		wprintf_s(L"Error:\nSQLSTATE=%ls, Native error=%ld, msg='%ls'", szSQLSTATE, nErr, msg);

		//swprintf_s(szData, sizeof(szData), L"Error:\nSQLSTATE=%ls, Native error=%ld, msg='%ls'", szSQLSTATE, nErr, msg);
		//MessageBox(NULL, szData, L"ODBC Error", MB_OK);
	}
}