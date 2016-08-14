#pragma once

namespace PACKET
{
	struct PaketHeader
	{
		short _id;
		short _bodySize;
	};


	const auto MAX_USER_ID_LEN = 5;
	const auto MAX_USER_PW_LEN = 10;
	const auto AUTH_TOKEN_LEN = 20;
	struct PaketLoginReq
	{
		wchar_t _id[MAX_USER_ID_LEN+1]; // +1은 nullptr을 위함
		wchar_t _pw[MAX_USER_PW_LEN+1];
		char	_authToken[AUTH_TOKEN_LEN + 1];
	};
	struct PacketLoginRes
	{

	};


	struct PacketRoomListReq
	{
	};
	
	const auto NUM_OF_ROOM_IN_ONE_REQ = 6;
	struct PacketRoomListRes
	{
		short	_pageNum;
	};

	const auto MAX_ROOM_CAPTION_LEN = 15;
	struct RoomInfoSmall
	{
		short	_roomNum;
		wchar_t _caption[MAX_ROOM_CAPTION_LEN+1];
		short	_userCountInRoom;
	};
}