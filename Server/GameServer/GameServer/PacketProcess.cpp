#include "stdafx.h"
#include "PacketProcess.h"


void PacketProcess::Init(TcpNet * pNetwork, UserManager * pUserMgr, LobbyManager * pLobbyMgr, ILog * pLogger)
{
	//[TODO] ...
}

void PacketProcess::Process(PacketInfo packetInfo)
{
	//[TODO] ...
}

ERROR_CODE PacketProcess::NtfSysCloseSesson(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::Login(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::Logout(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::ChannelEnter(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::ChannelRoomList(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::ChannelLeave(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomEnter(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomUserList(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomLeave(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::RoomChat(PacketInfo packetInfo)
{
	//[TODO] ...
	return ERROR_CODE();
}

ERROR_CODE PacketProcess::LeaveAllAndLogout(User * pUser, int SessionId)
{
	//[TODO] ...
	return ERROR_CODE();
}

