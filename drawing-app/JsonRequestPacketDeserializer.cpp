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

UploadPaintToRoomRequest JsonRequestPacketDeserializer::deserializeUploadPaintToRoomRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	UploadPaintToRoomRequest req;
	req.manager = json["manager"].get<std::string>();
	req.paint = getPaint(json["paint"]);
	req.roomId = json["roomId"].get<std::string>();
	return req;
}

GetUserPaintsNameRequest JsonRequestPacketDeserializer::deserializeGetUserPaintsRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	GetUserPaintsNameRequest req;
	req.username = json["username"].get<std::string>();
	return req;
}

AddLineToPaintRequest JsonRequestPacketDeserializer::deserialAddLineToPaintRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	AddLineToPaintRequest req;
	req.manager = json["manager"].get<std::string>();
	req.roomId = json["roomId"].get<std::string>();
	req.linesToAdd = getLines(json["line"]);
	return req;
}

GetPaintFromRoomRequest JsonRequestPacketDeserializer::deserialGetPaintFromRoomRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	GetPaintFromRoomRequest req;
	req.roomId = json["roomId"].get<std::string>();
	return req;
}

GetPaintByNameRequest JsonRequestPacketDeserializer::deserialGetPaintByNameRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	GetPaintByNameRequest req;
	req.user = json["user"].get<std::string>();
	req.paintName = json["paintName"].get<std::string>();
	return req;
}

SavePaintRequest JsonRequestPacketDeserializer::deserialSavePaintRequest(const Buffer& buffer)
{
	nlohmann::json json = nlohmann::json::parse(buffer.begin(), buffer.end());
	SavePaintRequest req;
	req.roomId = json["roomId"].get<std::string>();
	req.manager = json["manager"].get<std::string>();
	req.paintName = json["paintName"].get<std::string>();
	return req;
}

std::vector<Line> JsonRequestPacketDeserializer::getLines(const nlohmann::json& arr)
{
	std::vector<Line> Lines;
	Lines.reserve(arr.size());

	for (const auto& line : arr)
	{
		double x1 = line["Start"]["X"].get<double>();
		double y1 = line["Start"]["Y"].get<double>();
		double x2 = line["End"]["X"].get<double>();
		double y2 = line["End"]["Y"].get<double>();
		std::string color = line["ColorHex"].get<std::string>();

		Lines.emplace_back(
			std::make_pair(Coordinates(x1, y1), Coordinates(x2, y2)),
			color
		);
	}
	return Lines;
}

Paint JsonRequestPacketDeserializer::getPaint(const nlohmann::json& paintJson)
{
	std::string paintName = paintJson["paintName"].get<std::string>();
	std::vector<Line> lines = getLines(paintJson["lines"]);

	return Paint(lines, paintName);
}
