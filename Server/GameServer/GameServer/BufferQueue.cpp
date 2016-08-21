#include "stdafx.h"
#include "NetworkSetting.h"
#include "BufferQueue.h"

BufferQueue* BufferQueue::_instance = nullptr;
BufferQueue* BufferQueue::GetInstance()
{
	if (_instance == nullptr)
		_instance = new BufferQueue();
	return _instance;
}

void BufferQueue::Init(NetworkSetting setting)
{
	// 방어코드
	if (_bufferPool.size() != 0)
		return;
	// 버퍼 풀 구성
	for (unsigned i = 0; i < setting._maxBufferCount; ++i)
	{
		_bufferPool.emplace_back(new char[setting._maxBufferSize]);
	}
}

// 사용할 버퍼를 풀에서 하나 얻어온다.
IOCPBuffer BufferQueue::GetBufferThreadSafe()
{
	// multiple readers on one container are safe!! 신난다
	while(true)
	{
		_mutex.lock();
		auto isEmpty = _bufferPool.empty();
		auto result = IOCPBuffer{};
		if (isEmpty == false)
		{
			result = _bufferPool.front();
			_bufferPool.pop_front();
		}
		_mutex.unlock();
		// pop 성공 했으면 리턴
		if (isEmpty == false)
			return result;
	}
}

// 사용 완료한 버퍼를 버퍼 풀에 다시 넣는다
void BufferQueue::ReturnBufferThreadSafe(IOCPBuffer buffer)
{
	_mutex.lock();
	_bufferPool.emplace_back(buffer);
	_mutex.unlock();
}
