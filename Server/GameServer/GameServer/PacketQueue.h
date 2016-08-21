#pragma once

using namespace COMMON;


class PacketQueue
{
public:
	RecvPacketInfo	ReadFront();
	void			PopFront();
	void			PushBack(RecvPacketInfo& recvPacket);
private:

public:
private:
	std::deque<RecvPacketInfo>	_packetDeque;
	std::mutex					_mutex;
};