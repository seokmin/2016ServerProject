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
		ROOM_LEAVE_USER_NTF = 33, // change나 leave나 한끗차이인데 둘다 leave user ntf는 필요하므로 묶었음

		ROOM_CHANGE_REQ = 41,
		ROOM_CHANGE_RES = 42,

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

	const auto MAX_PACKET_SIZE = 1024;
	const auto MAX_USER_ID_LEN = 5;
	const auto MAX_USER_PW_LEN = 10;
	const auto AUTH_TOKEN_LEN = 20;
	const auto MAX_ROOM_CAPTION_LEN = 15;

	struct PacketHeader
	{
		PACKET_ID	_id;
		short		_bodySize;
	};
	const auto PACKET_HEADER_SIZE = sizeof(PacketHeader);

	struct PacketBase
	{
		short _errorCode = ERROR_CODE::NONE;
		void SetErrCode(ERROR_CODE errNum) { _errorCode = (short)errNum; };
	};

	// 로그인 서버가 알려준 채널 서버 주소에 대해 인증키를 보내며 로그인을 요청한다.
	struct PacketRoomEnterReq
	{
		char	_authToken[AUTH_TOKEN_LEN + 1];
	};

	// _errorCode가 ERROR_CODE::NONE이면 성공으로 간주
	struct PacketRoomEnterRes : PacketBase
	{
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
