#pragma once
#include <mutex>
#include <deque>

using IOCPBuffer = char*;
struct NetworkSetting;


class BufferQueue
{
public:
	static BufferQueue*		GetInstance();
	void					Init(NetworkSetting setting);
	IOCPBuffer				GetBufferThreadSafe();
	void					ReturnBufferThreadSafe(IOCPBuffer buffer);
private:
public:
private:
	static BufferQueue*		_instance;
	
	std::mutex				_mutex;

	std::deque<IOCPBuffer>	_bufferPool;
};