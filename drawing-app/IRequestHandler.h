#pragma once
#include "Pch.h"
#include "RequestResult.h"

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

class IRequestHandler
{
public:

	virtual bool isRequestRelevant(RequestInfo& req) = 0;
	virtual RequestResult handlerRequest(RequestInfo& req) = 0;
};