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

		MAX = 255,
	};

	const auto MAX_USER_ID_LEN = 5;
	const auto MAX_USER_PW_LEN = 10;
	const auto AUTH_TOKEN_LEN = 20;
	const auto MAX_ROOM_CAPTION_LEN = 15;

	struct PaketHeader
	{
		short _id;
		short _bodySize;
	};

	struct PaketBase
	{
		short _errorCode = ERROR_CODE::NONE;
		void SetErrCode(ERROR_CODE errNum) { _errorCode = (short)errNum; };
	};

	// 로그인 서버가 알려준 채널 서버 주소에 대해 id, 패스워드, 인증키를 보내며 로그인을 요청한다.
	struct PaketLoginReq
	{
		wchar_t _id[MAX_USER_ID_LEN+1];
		wchar_t _pw[MAX_USER_PW_LEN+1];
		char	_authToken[AUTH_TOKEN_LEN + 1];
	};

	// _errorCode가 ERROR_CODE::NONE이면 성공으로 간주
	struct PacketLoginRes : PaketBase
	{
	};

	// 특정 페이지에 대한 룸 정보를 요청한다.
	struct PacketRoomListReq
	{
		short _pageNum;
		//short _pageSize; // if 0, get full list
	};

	// 로비에 보여지기 위한 룸 정보
	struct RoomInfoSmall : PaketBase
	{
		short	_roomNum;
		wchar_t _caption[MAX_ROOM_CAPTION_LEN + 1];
		short	_userCountInRoom;
	};

	// 해당 페이지에 대한 룸 정보를 모두 보내준다.
	const auto NUM_OF_ROOMS_IN_PAGE = 6;
	struct PacketRoomListRes : PaketBase
	{
		RoomInfoSmall _roomInfos[NUM_OF_ROOMS_IN_PAGE];
		//short	_pageNum;
	};

	// 이건 서버에서만 쓰지 않을까요?
	struct RecvPacketInfo
	{
		int SessionIndex = 0;
		short PacketId = 0;
		short PacketBodySize = 0;
		char* pRefData = 0;
	};
}
