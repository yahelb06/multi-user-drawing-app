#pragma once
#include "Pch.h"

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

struct AddPaintToRoomResponse
{
	unsigned int status;

};

struct GetUsersInRoomResponse
{
	std::vector <std::string> usersInRoom;
};