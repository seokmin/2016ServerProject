#pragma once

// Connecting_with_SQLConnect.cpp
// compile with: user32.lib odbc32.lib
#include <sqlext.h>
#include <mbstring.h>

#define MAX_DATA 100
#define MYSQLSUCCESS(rc) ((rc == SQL_SUCCESS) || (rc == SQL_SUCCESS_WITH_INFO) )

class MySQLMangager
{
public:
	MySQLMangager();
	~MySQLMangager();
	RETCODE rc; // ODBC return code
	HENV henv; // Environment   
	HDBC hdbc; // Connection handle
	HSTMT hstmt; // Statement handle

	SQLWCHAR szData[MAX_DATA]; // Returned data storage
	SQLLEN cbData; // Output length of data
	SQLWCHAR chr_ds_name[SQL_MAX_DSN_LENGTH]; // Data source name

public:
	RETCODE sqlconn(); // Allocate env, stat, and conn
	RETCODE sqlexec(SQLWCHAR * cmdstr, int n_args, ...); // Execute SQL statement
	void sqldisconn(); // Free pointers to env, stat, conn, and disconnect
	void error_out(); // Displays errors
};
