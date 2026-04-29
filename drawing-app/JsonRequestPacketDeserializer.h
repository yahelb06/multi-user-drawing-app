#pragma once
#include "Pch.h"
#include "IRequestHandler.h"
#include "Json.hpp"

class JsonRequestPacketDeserializer
{
public:

	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignUpRequest deserializeSignUpRequest(const Buffer& buffer);
	static RemoveUserRequest deserializeRemoveUserRequest(const Buffer& buffer);
	static UserLogOutRequest deserializeLogOutUserRequest(const Buffer& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const Buffer& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const Buffer& buffer);
	static RoomLogOutRequest deserializeRoomLogOutRequest(const Buffer& buffer);
	static AddUserRequest deserializeAddUserRequest(const Buffer& buffer);
	static RemoveUserFromRoomRequest deserializeRemoveUserFromRoomRequest(const Buffer& buffer);
};