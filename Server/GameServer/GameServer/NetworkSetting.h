#pragma once

struct NetworkSetting
{
	int			_portNum;
//	int			_maxIocpBufferSize; // max iocp buffer size
//	unsigned	_maxBufferCount;
	unsigned	_maxSessionCount;
	unsigned	_maxSessionRecvBufferSize;
	int			_backLog;
};