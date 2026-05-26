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
	std::string userToAccept;
	std::string roomId;
};

struct AcceptUserResponse
{
	unsigned int status;
	std::string roomId;
	std::vector<std::string> usersInRoom;
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
	std::vector<Line> paintLines;
};

struct GetUsersInRoomResponse
{
	std::vector <std::string> usersInRoom;
};

struct GetUserPaintsNameResponse
{
	unsigned int status;
	std::vector<std::string> paintsName;
};

struct AddLinesToPaintResponse
{
	unsigned int status;
};

struct GetPaintFromRoomResponse
{
	std::vector<Line> vecLines;
};