#pragma once
#include "stdafx.h"
class DBmanager
{
public:
	DBmanager();
	virtual ~DBmanager();
	
	COMMON::ERROR_CODE SubmitState(int max, int count);

private:
};

