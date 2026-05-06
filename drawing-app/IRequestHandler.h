#pragma once
#include "Pch.h"
#include "RequestResult.h"
#include "Line.h"

struct RequestInfo
{
	char id;
	time_t arrivalTime;
	Buffer buffer;
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

struct AcceptUserRequest
{
	std::string userToAccept;
	std::string roomId;
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

struct AddPaintToRoomRequest
{
	PaintRoomData data;
	std::vector<Line> LinesInPaint;
};

class IRequestHandler
{
public:

	virtual bool isRequestRelevant(RequestInfo& req) = 0;
	virtual RequestResult handlerRequest(RequestInfo& req) = 0;
};