#pragma once
#include "Pch.h"

class IRequestHandler;

struct RequestResult
{
	Buffer response;
	IRequestHandler* newHandler;
};