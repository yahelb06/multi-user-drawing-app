#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	LoginRequest req;
	req.username = json["username"].get<std::string>();
	req.password = json["password"].get<std::string>();
	return req;
}

SignUpRequest JsonRequestPacketDeserializer::deserializeSignUpRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	SignUpRequest req;
	req.username = json["username"].get<std::string>();
	req.password = json["password"].get<std::string>();
	req.mail = json["mail"].get<std::string>();
	return req;
}

RemoveUserRequest JsonRequestPacketDeserializer::deserializeRemoveUserRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	RemoveUserRequest req;
	req.username = json["username"].get<std::string>();
	return req;
}

UserLogOutRequest JsonRequestPacketDeserializer::deserializeLogOutUserRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	UserLogOutRequest req;
	req.username = json["username"].get<std::string>();
	return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	CreateRoomRequest req;
	req.username = json["username"].get<std::string>();
	return req;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	JoinRoomRequest req;
	req.username = json["username"].get<std::string>();
	req.roomId = json["roomId"].get<std::string>();
	return req;
}

RoomLogOutRequest JsonRequestPacketDeserializer::deserializeRoomLogOutRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	RoomLogOutRequest req;
	req.username = json["username"].get<std::string>();
	req.roomId = json["roomId"].get<std::string>();
	return req;
}

AddUserRequest JsonRequestPacketDeserializer::deserializeAddUserRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	AddUserRequest req;
	req.manager = json["manager"].get<std::string>();
	req.userToAdd = json["userToAdd"].get<std::string>();
	req.roomId = json["roomId"].get<std::string>();
	req.accept = json["accept"].get<bool>();
	return req;
}

RemoveUserFromRoomRequest JsonRequestPacketDeserializer::deserializeRemoveUserFromRoomRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	RemoveUserFromRoomRequest req;
	req.manager = json["manager"].get<std::string>();
	req.userToRemove = json["userToRemove"].get<std::string>();
	req.roomId = json["roomId"].get<std::string>();
	return req;
}

RemovePaintFromRoomRequest JsonRequestPacketDeserializer::deserializeRemovePaintFromRoomRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	RemovePaintFromRoomRequest req;
	req.manager = json["manager"].get<std::string>();
	req.paintName = json["paintName"].get<std::string>();
	req.roomId = json["roomId"].get<std::string>();
	return req;
}
