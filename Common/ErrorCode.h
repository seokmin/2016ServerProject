#pragma once
namespace COMMON
{
	enum ERROR_CODE : short
	{
		NONE = 0,

		UNASSIGNED_ERROR = 201,

		MAIN_INIT_NETWORK_INIT_FAIL = 206,

		USER_MGR_ID_DUPLICATION = 211,
		USER_MGR_MAX_USER_COUNT = 212,
		USER_MGR_INVALID_SESSION_INDEX = 213,
		USER_MGR_NOT_CONFIRM_USER = 214,
		USER_MGR_REMOVE_INVALID_SESSION = 221,

		CHANNEL_ENTER_INVALID_DOMAIN = 231,
		CHANNEL_ENTER_INVALID_CHANNEL_INDEX = 232,
		CHANNEL_ENTER_USER_DUPLICATION = 234,
		CHANNEL_ENTER_MAX_USER_COUNT = 235,
		
		CHANNEL_ROOM_LIST_INVALID_START_ROOM_INDEX = 241,
		CHANNEL_ROOM_LIST_INVALID_DOMAIN = 242,
		CHANNEL_ROOM_LIST_INVALID_CHANNEL_INDEX = 243,

		CHANNEL_LEAVE_INVALID_DOMAIN = 261,
		CHANNEL_LEAVE_USER_NVALID_UNIQUEINDEX = 263,

		ROOM_ENTER_INVALID_DOMAIN = 271,
		ROOM_ENTER_INVALID_LOBBY_INDEX = 272,
		ROOM_ENTER_INVALID_ROOM_INDEX = 273,
		ROOM_ENTER_CREATE_FAIL = 274,
		ROOM_ENTER_NOT_CREATED = 275,
		ROOM_ENTER_MEMBER_FULL = 276,
		ROOM_ENTER_EMPTY_ROOM = 277,

		ROOM_USER_LIST_INVALID_DOMAIN = 280,
		ROOM_USER_LIST_INVALID_LOBBY_INDEX = 281,
		ROOM_USER_LIST_INVALID_ROOM_INDEX = 282,

		ROOM_LEAVE_INVALID_DOMAIN = 286,
		ROOM_LEAVE_INVALID_LOBBY_INDEX = 287,
		ROOM_LEAVE_INVALID_ROOM_INDEX = 288,
		ROOM_LEAVE_NOT_CREATED = 289,
		ROOM_LEAVE_NOT_MEMBER = 290,

		ROOM_GAME_INVALID_DOMAIN = 296,
		ROOM_GAME_INVALID_PLAY = 297,
		ROOM_GAME_INVALID_ROOM_INDEX = 298,

		ROOM_EMOTION_INVALID_DOMAIN = 301,
		ROOM_EMOTION_INVALID_EMOTION = 302,
		ROOM_EMOTION_INVALID_ROOM_INDEX = 303,
	};
	/*
	Result Status
	0 : No Error
	10 : Invalid Password
	11 : Create Account Error
	12 : Create Successful
	13 : Get User Error
	*/
	enum RESULT_LOGIN : short
	{
		SUCCESS_LOGIN = 0,
		SUCCESS_CREATE_ACCOUNT = 12,
		ERR_INVALID_PASSWORD = 10,
		ERR_CREATE_ACCOUNT = 11,
		ERR_GET_USER = 13
	};
};