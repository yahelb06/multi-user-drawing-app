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

LogOutRequest JsonRequestPacketDeserializer::deserializeLogOutUserRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	LogOutRequest req;
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
