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
	static RemovePaintFromRoomRequest deserializeRemovePaintFromRoomRequest(const Buffer& buffer);
	static UploadPaintToRoomRequest deserializeUploadPaintToRoomRequest(const Buffer& buffer);
	static GetUserPaintsNameRequest deserializeGetUserPaintsRequest(const Buffer& buffer);
	static AddLineToPaintRequest deserialAddLineToPaintRequest(const Buffer& buffer);
	static GetPaintFromRoomRequest deserialGetPaintFromRoomRequest(const Buffer& buffer);
	static GetPaintByNameRequest deserialGetPaintByNameRequest(const Buffer& buffer);
	static SavePaintRequest deserialSavePaintRequest(const Buffer& buffer);

private:
	static std::vector<Line> getLines(const nlohmann::json& arr);
	static Paint getPaint(const nlohmann::json& paintJson);
};