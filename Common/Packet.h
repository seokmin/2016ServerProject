#pragma once
#include "ErrorCode.h"

namespace COMMON
{
	// 21 이상에서 시작해야함.
	enum PACKET_ID : short
	{
		ROOM_ENTER_REQ = 21, //방 조인 요청
		ROOM_ENTER_NOW_REQ = 22, //바로 조인 요청.
		ROOM_ENTER_RES = 23,

		ROOM_ENTER_USER_LIST_REQ = 25,
		ROOM_ENTER_USER_LIST_RES = 26,
		ROOM_ENTER_USER_NTF = 27,

		ROOM_LEAVE_REQ = 31,
		ROOM_LEAVE_RES = 32,
		ROOM_LEAVE_USER_NTF = 33,

		CHANNEL_JOIN_REQ = 41,
		CHANNEL_JOIN_RES = 42,
		CHANNEL_JOIN_NTF = 43,

		CHANNEL_LEAVE_REQ = 51,
		CHANNEL_LEAVE_RES = 52,
		CHANNEL_LEAVE_USER_NTF = 53,


		GAME_BET_REQ = 101,
		GAME_BET_RES = 102,
		GAME_BET_NTF = 103,

		GAME_PLAY_REQ = 111,
		GAME_PLAY_RES = 112,
		GAME_PLAY_NTF = 113,

		GAME_EMOTION_REQ = 121,
		GAME_EMOTION_RES = 122,
		GAME_EMOTION_NTF = 123,

	};

	const auto MAX_USER_ID_LEN = 5;
	const auto MAX_USER_PW_LEN = 10;
	const auto AUTH_TOKEN_LEN = 20;
	const auto NUM_OF_ROOM_IN_ONE_REQ = 6;
	const auto MAX_ROOM_CAPTION_LEN = 15;

	struct PaketHeader
	{
		short _id;
		short _bodySize;
	};

	//로그인은 로그인서버랑 HTTP로 하기로 한거 아님?
	struct PaketBase
	{
		short _err_code = ERROR_CODE::NONE;
		void SetErrCode(ERROR_CODE errNum) { _err_code = (short)errNum; };
	};

	struct PaketLoginReq
	{
		wchar_t _id[MAX_USER_ID_LEN+1];
		wchar_t _pw[MAX_USER_PW_LEN+1];
		char	_authToken[AUTH_TOKEN_LEN + 1];
	};

	struct PacketLoginRes : PaketBase
	{
	};

	struct PacketRoomListReq
	{
		short _pageSize;
	};

	struct PacketRoomListRes : PaketBase
	{
		short	_pageNum;
	};

	struct RoomInfoSmall : PaketBase
	{
		short	_roomNum;
		wchar_t _caption[MAX_ROOM_CAPTION_LEN+1];
		short	_userCountInRoom;
	};
}
