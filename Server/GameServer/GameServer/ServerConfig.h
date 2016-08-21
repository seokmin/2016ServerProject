#pragma once
#include "Common.h"

using namespace COMMON;

class ServerConfig
{
public:

	static const int MAX_NET_ERROR_STRING_COUNT = 64;
	static const int MAX_NAME_LEN = 19;
	static const int MAX_IP_LEN = 32; // IP 문자열 최대 길이
	static const int MAX_PACKET_SIZE = 1024; // 최대 패킷 크기
	static const int PACKET_HEADER_SIZE = COMMON::PACKET_HEADER_SIZE;

	static const int MAX_USERCOUNT_PER_CHANNEL = 100;
	static const int MAX_ROOMCOUNT = 20;
	static const int MAX_USERCOUNT_PER_ROOM = COMMON::MAX_USERCOUNT_PER_ROOM;

	WCHAR SERVERNAME[MAX_NAME_LEN + 1] = L"testServer";
	WCHAR IP[MAX_IP_LEN + 1] = L"10.73.38.134";
	unsigned short Port = 12800;
	unsigned int minBet = 100;
	unsigned int maxBet = 1000;


	int BackLogCount;

	int ExtraClientCount = 20; // 넘쳤다는걸 알려주기 위한 여유분..

	short MaxClientSockOptRecvBufferSize;
	short MaxClientSockOptSendBufferSize;
	short MaxClientRecvBufferSize;
	short MaxClientSendBufferSize;

	// 에러 코드는 1 ~ 200까지 사용한다.
	enum NET_ERROR_CODE : short
	{
		NONE = 0,

		SERVER_SOCKET_CREATE_FAIL = 11,
		SERVER_SOCKET_SO_REUSEADDR_FAIL = 12,
		SERVER_SOCKET_BIND_FAIL = 14,
		SERVER_SOCKET_LISTEN_FAIL = 15,

		SEND_CLOSE_SOCKET = 21,
		SEND_SIZE_ZERO = 22,
		CLIENT_SEND_BUFFER_FULL = 23,
		CLIENT_FLUSH_SEND_BUFF_REMOTE_CLOSE = 24,
		ACCEPT_API_ERROR = 26,
		ACCEPT_MAX_SESSION_COUNT = 27,
		RECV_API_ERROR = 32,
		RECV_BUFFER_OVERFLOW = 32,
		RECV_REMOTE_CLOSE = 33,
		RECV_PROCESS_NOT_CONNECTED = 34,
		RECV_CLIENT_MAX_PACKET = 35,
	};

	struct NetError
	{
		NetError(NET_ERROR_CODE code)
		{
			Error = code;
		}

		NET_ERROR_CODE Error = NET_ERROR_CODE::NONE;
		wchar_t Msg[MAX_NET_ERROR_STRING_COUNT] = { 0, };
		int Value = 0;
	};

	struct ClientSession
	{
		bool IsConnected() { return SocketFD > 0 ? true : false; }

		void Clear()
		{
			Seq = 0;
			SocketFD = 0;
			IP[0] = '\0';
			RemainingDataSize = 0;
			SendSize = 0;
		}

		int Index = 0;
		long long Seq = 0;
		int		SocketFD = 0;
		char    IP[MAX_IP_LEN] = { 0, };

		char*   pRecvBuffer = nullptr;
		int     RemainingDataSize = 0;

		char*   pSendBuffer = nullptr;
		int     SendSize = 0;
	};

	enum SOCKET_CLOSE_CASE : short
	{
		SESSION_POOL_EMPTY = 1,
		SELECT_ERROR = 2,
		SOCKET_RECV_ERROR = 3,
		SOCKET_RECV_BUFFER_PROCESS_ERROR = 4,
		SOCKET_SEND_ERROR = 5,
	};

	enum class PACKET_ID : short
	{
		NTF_SYS_CONNECT_SESSION = 2,
		NTF_SYS_CLOSE_SESSION = 3,
	};
};

