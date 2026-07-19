#pragma once
#include "Pch.h"
#include "RequestResult.h"
#include "Paint.h"

struct RequestInfo
{
	char id;
	time_t arrivalTime;
	Buffer buffer;
	SOCKET socket;
};

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignUpRequest
{
	std::string username;
	std::string password;
	std::string mail;
};

struct RemoveUserRequest
{
	std::string username;
};

struct UserLogOutRequest
{
	std::string username;
};

struct CreateRoomRequest
{
	std::string username;
};

struct JoinRoomRequest
{
	std::string username;
	std::string roomId;
};

struct RoomLogOutRequest
{
	std::string username;
	std::string roomId;
};

struct AddUserRequest
{
	std::string manager;
	std::string userToAdd;
	std::string roomId;
	bool accept;
};

struct RemoveUserFromRoomRequest
{
	std::string manager;
	std::string userToRemove;
	std::string roomId;
};

struct PaintRoomData
{
	std::string manager;
	std::string roomId;
	std::string paintName;
};

struct RemovePaintFromRoomRequest
{
	PaintRoomData data;
};

struct UploadPaintToRoomRequest
{
	std::string manager;
	std::string roomId;
	Paint paint;
};

struct GetPaintByNameRequest
{
	std::string user;
	std::string paintName;
};

struct GetUserPaintsNameRequest
{
	std::string username;
};

struct AddLineToPaintRequest
{
	std::string manager;
	std::string roomId;
	std::vector<Line> linesToAdd;
};

struct GetPaintFromRoomRequest
{
	std::string roomId;
};

struct SavePaintRequest
{
	std::string roomId;
	std::string manager;
	std::string paintName;
};

class IRequestHandler
{
public:

	virtual bool isRequestRelevant(RequestInfo& req) = 0;
	virtual RequestResult handlerRequest(RequestInfo& req) = 0;
};