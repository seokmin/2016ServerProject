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
	struct PaketLoginReq
	{
		wchar_t _id[MAX_USER_ID_LEN+1]; // +1은 nullptr을 위함
		wchar_t _pw[MAX_USER_PW_LEN+1];

	};
}