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

class IRequestHandler
{
	virtual bool isRequestRelevant(RequestInfo& info) = 0;
	virtual RequestResult handlerRequest(RequestInfo& info) = 0;
};