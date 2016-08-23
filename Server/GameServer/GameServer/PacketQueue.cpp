#include "stdafx.h"
#include <deque>
#include <mutex>

#include "PacketQueue.h"

// 제일 앞의 결과를 보여준다
RecvPacketInfo PacketQueue::ReadFront()
{
	if (this->IsEmpty())
	{
		auto nullPacket = RecvPacketInfo{};
		nullPacket.PacketId = PACKET_ID::NULL_PACKET;
		return nullPacket;
	}
	return _packetDeque.front();
}

// 이 함수는 sync-safe 하지 않다. 여러군데에서 동시에 부르면 결과를 보장하지 못한다.
void PacketQueue::PopFront()
{
	if (_packetDeque.empty())
		return;
	_mutex.lock();
	auto packet2Delete = _packetDeque.front();
	_packetDeque.pop_front();
	if (packet2Delete.pRefData != nullptr)
	{
		delete[] packet2Delete.pRefData;
		packet2Delete.pRefData = nullptr;
	}
	_mutex.unlock();
}

// 이 함수는 sync-safe, thread-safe하다.
// 아무데서나 막 불러도 괜찮다. 현재 사용 용도는 네트워크 레이어에서 부를 용도
void PacketQueue::PushBack(RecvPacketInfo& recvPacket)
{
	// 패킷 복사
	auto copiedPacket = RecvPacketInfo{recvPacket};
	auto newBody = new char[copiedPacket.PacketBodySize];
	copiedPacket.pRefData = newBody;
	auto bodySize = recvPacket.PacketBodySize;
	memcpy_s(newBody, bodySize, recvPacket.pRefData, bodySize);
	
	//새로 만든 패킷을 큐에 넣어준다.
	_mutex.lock();
	_packetDeque.push_back(copiedPacket);
	_mutex.unlock();
}

bool PacketQueue::IsEmpty()
{
	return _packetDeque.empty();
}
