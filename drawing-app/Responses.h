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

struct LogOutResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
};