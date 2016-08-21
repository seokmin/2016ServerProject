#pragma once
#include <mutex>
#include <deque>

using namespace COMMON;


class PacketQueue
{
public:
	RecvPacketInfo	ReadFront();
	void			PopFront();
	void			PushBack(RecvPacketInfo& recvPacket);
	~PacketQueue()
	{
		auto a = 1;
	}
private:
public:
private:
	std::deque<RecvPacketInfo>	_packetDeque;
	std::mutex					_mutex;
};