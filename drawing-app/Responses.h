#pragma once
#include "Pch.h"
#include "Line.h"

struct ErrResponse
{
	std::string message;
};

struct LoginResponse
{
	unsigned int status;
};

struct SignUpResponse
{
	unsigned int status;
};

struct RemoveUserResponse
{
	unsigned int status;
};

struct UserLogOutResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	std::string roomId;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct RoomLogOutResponse
{
	unsigned int status;
};

struct AddUserResponse
{
	unsigned int status;
};

struct AcceptUserResponse
{
	unsigned int status;
	std::string roomId;
	std::vector<std::string> usersInRoom;
	std::vector<Line> vecLines;
};

struct RemoveUserFromRoomResponse
{
	unsigned int status;
};

struct RemovePaintFromRoomResponse
{
	unsigned int status;
};

struct UploadPaintToRoomResponse
{
	unsigned int status;
};

struct GetUsersInRoomResponse
{
	std::vector<std::string> usersInRoom;
};

struct GetUserPaintsNameResponse
{
	std::vector<std::string> paintsName;
};

struct AddLinesToPaintResponse
{
	unsigned int status;
};

/*struct GetPaintFromRoomResponse
{
	std::vector<Line> vecLines;
};*/

struct GetNewLinesResponse
{
	std::vector<Line> newLines;
};

struct SavePaintResponse
{
	unsigned int status;
};