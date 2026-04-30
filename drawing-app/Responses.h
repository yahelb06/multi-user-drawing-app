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
	unsigned int status;
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

struct RemoveUserFromRoomResponse
{
	unsigned int status;
};

struct RemovePaintFromRoomResponse
{
	unsigned int status;
};