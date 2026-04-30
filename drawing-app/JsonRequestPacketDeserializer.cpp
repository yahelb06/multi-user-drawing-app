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
	req.data.manager = json["manager"].get<std::string>();
	req.data.paintName = json["paintName"].get<std::string>();
	req.data.roomId = json["roomId"].get<std::string>();
	return req;
}

AddPaintToRoomRequest JsonRequestPacketDeserializer::deserializeAddPaintToRoomRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	AddPaintToRoomRequest req;
	req.data.manager = json["manager"].get<std::string>();
	req.data.paintName = json["paintName"].get<std::string>();
	req.data.roomId = json["roomId"].get<std::string>();
	req.LinesInPaint = getLines(json["LinesInPaint"]);
	return req;
}

std::vector<Line> JsonRequestPacketDeserializer::getLines(const nlohmann::json& arr)
{
	std::vector<Line> Lines;
	Lines.reserve(arr.size());

	for (const auto& line : arr)
	{
		Lines.emplace_back(
			std::make_pair(Coordinates(arr["x1"], arr["y1"]), Coordinates(arr["x2"], arr["y2"])),
			arr["color"].get<std::string>()
		);
	}
	return Lines;
}
