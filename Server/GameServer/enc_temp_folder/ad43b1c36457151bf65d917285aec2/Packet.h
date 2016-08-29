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

		GAME_BET_COUNTER_NTF = 91,
		
		GAME_BET_REQ = 92,
		//GAME_BET_RES = 93,
		GAME_BET_NTF = 94,

		GAME_START_NTF			= 95,
		GAME_CHANGE_TURN_NTF	= 96,

		GAME_CHOICE_REQ = 97,
		GAME_CHOICE_NTF = 98,

		GAME_DEALER_RESULT_NTF = 101,
		GAME_ANI_COMPLETE_REQ = 102,

		GAME_EMOTION_REQ = 121,
		GAME_EMOTION_RES = 122,
		GAME_EMOTION_NTF = 123,

		CLOSE_SESSION = 254,
		MAX = 255,
	};

	enum class ChoiceKind
	{
		STAND		= 0,
		HIT			= 1,
		DOUBLE_DOWN = 2,
		SPLIT		= 3
	};

	struct CardInfo
	{
		enum CardShape : short
		{
			EMPTY = 0, // 카드 없음

			SPADE = 1,
			DIAMOND = 2,
			HEART = 3,
			CLOVER = 4,
		} _shape = EMPTY;

		short _number = 0; // J = 11, Q = 12, K = 13
	};

	struct HandInfo
	{
		CardInfo _cardList[8];
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
		CardInfo _openedCardList[8];
	};

	struct UserInfo
	{
		int _pokeNum = 0; // 1 ~ 151
		wchar_t _name[MAX_USER_ID_LEN + 1] = L"";
		int _totalMony = 0;
		int _betMoney = 0;
		int _curHand = 0;
		bool _isSpit = false;
		HandInfo _hands[MAX_HAND];
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
		int _slot = -1;
		DealerInfo _dealerinfo;
		UserInfo _users[MAX_USERCOUNT_PER_ROOM];
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


	// 게임 로직 부분
	struct PacketGameBetCounterNtf
	{
		int _countTime = 15;
		unsigned int minBet = 100;
		unsigned int maxBet = 1000;
	};

	struct PacketGameBetReq
	{
		int _betMoneyReq;
	};
// 	struct PacketGameBetRes // TODO : 안쓸 가능성이 높음 지워야함
// 	{
// 		int _betMoneyRes;
// 	};
	struct PacketGameBetNtf
	{
		int _betSlot;
		int _betMoney;
	};

	struct PacketGameStartNtf
	{
		int _startSlotNum; // 시작하는 사람의 자리를 알려줌
		int _turnCountTime = 10; // 힛, 스탠드 고를 시간 10초

		HandInfo _handInfo[5]; // 각자의 핸드 0 정보. (1은 안줌~)
		CardInfo _dealerCard; // 딜러카드 한장만 오픈
	};

	struct PacketGameChangeTurnNtf
	{
		int _slotNum;
		int _handNum = 0;
		int _waitingTime = 10;
	};
	
	struct PacketGameChoiceReq
	{
		ChoiceKind _choice; // 스플릿, 힛, 스탠드 등 고른 행동을 알려줌
	};

	struct PacketGameChoiceNtf
	{
		int			_slotNum;
		ChoiceKind	_choice; // 모두에게 노티를 줘서 화면을 바꿈
		CardInfo	_recvCard; // hit, double down일 때만 사용한다
		int			_handNum; // front hand 또는 back hand
		int			_betMoney; // 초이스 후 걸게되는 돈 (더블다운/스플릿)
		int			_currentMoney; // 초이스 후 남는 돈
		int			_waitingTime;
	};

	struct PacketGameDealerResultNtf
	{
		DealerInfo  _dealerResult;
		int			_plusMoney[5]; // 각  슬롯마다 번 돈
	};

	struct PacketGameAnimCompleteReq
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