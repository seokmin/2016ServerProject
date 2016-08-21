#pragma once

struct NetworkSetting
{
	int			_portNum;
	int			_maxBufferSize; // max iocp buffer size
	unsigned	_maxBufferCount;
	unsigned	_maxSessionCount;
	int			_backLog;
};