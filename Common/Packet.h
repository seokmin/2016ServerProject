#pragma once
#include "ErrorCode.h"

namespace COMMON
{

	const auto MAX_PACKET_SIZE = 1024;
	const auto MAX_USER_ID_LEN = 5;
	const auto MAX_USER_PW_LEN = 10;
	const auto AUTH_TOKEN_LEN = 45;
	const auto MAX_ROOM_CAPTION_LEN = 15;
	const auto MAX_HAND = 2; // maximum hands by split

	struct CardInfo
	{
		enum class CardShape {
			EMPTY = 0, // 카드 없음

			SPADE = 1,
			DIAMOND = 2,
			HEART = 3,
			CLOVER = 4,
		} _shape;

		short _number; // J = 11, Q = 12, K = 13
	};


	struct HandInfo
	{
		CardInfo _cardList[10];
		enum HandState {
			CURRENT = 0,
			BURST = 1,
			STAND = 2,
			BLACKJACK = 3,
		} _handState;
		bool _isDoubledown;
	};

	struct DealerInfo
	{
		CardInfo _openedCardList[11];
		int _hiddenCard;  // How many hidden cards is there? maybe 0 or 1;
	};

	struct UserInfo
	{
		int _pokeNum; // 1 ~ 151
		wchar_t _name[MAX_USER_ID_LEN + 1];
		int _totalMony;
		int _betMoney;
		HandInfo _hands[MAX_HAND];
	};


	// 21 이상에서 시작해야함.
	enum PACKET_ID : short
	{
		NULL_PACKET = 0,
		ROOM_ENTER_REQ = 21,
		ROOM_ENTER_RES = 22,

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

		CLOSE_SESSION = 254,
		MAX = 255,
	};

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
		int _roomNum;
	};

	struct PacketRoomUserlistReq
	{

	};

	static const int MAX_USERCOUNT_PER_ROOM = 5;
	struct PacketRoomUserlistRes : PacketBase
	{
		int _roomNum;
		DealerInfo _dealerinfo;
		UserInfo _users[MAX_USERCOUNT_PER_ROOM-1];
	};

	struct PacketRoomEnterNtf
	{
		UserInfo _enterUser;
		int _slotNum;
	};

	struct PacketRoomLeaveReq
	{
	};
	struct PacketRoomLeaveRes : PacketBase
	{
	};
	struct PacketRoomLeaveNtf
	{
		int _slotNum;
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