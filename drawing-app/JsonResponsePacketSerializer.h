#pragma once
#include "Pch.h"
#include "Json.hpp"
#include "Responses.h"

class JsonResponsePacketSerializer
{
private:
	static void getSizeIntoBuffer(int size, Buffer& buffer);


public:

	static Buffer serializeResponse(ErrResponse& res);
	static Buffer serializeResponse(LoginResponse& res);
	static Buffer serializeResponse(SignUpResponse& res);
	static Buffer serializeResponse(RemoveUserResponse& res);
	static Buffer serializeResponse(UserLogOutResponse& res);
	static Buffer serializeResponse(CreateRoomResponse& res);
	static Buffer serializeResponse(JoinRoomResponse& res);
	static Buffer serializeResponse(RoomLogOutResponse& res);
	static Buffer serializeResponse(AddUserResponse& res);
	static Buffer serializeResponse(AcceptUserResponse& res);
	static Buffer serializeResponse(RemoveUserFromRoomResponse& res);
	static Buffer serializeResponse(RemovePaintFromRoomResponse& res);
	static Buffer serializeResponse(AddPaintToRoomResponse& res);
	static Buffer serializeResponse(GetUsersInRoomResponse& res);
};