#pragma once
#include "Pch.h"
#include "RequestResult.h"

struct RequestInfo
{
	char id;
	time_t arrivalTime;
	Buffer buffer;
};

class IRequestHandler
{
	virtual bool isRequestRelevant(RequestInfo& req) = 0;
	virtual RequestResult handlerRequest(RequestInfo& req) = 0;
};