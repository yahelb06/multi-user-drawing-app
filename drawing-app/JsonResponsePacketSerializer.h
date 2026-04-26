#pragma once
#include "Pch.h"
#include "Json.hpp"
#include "Responses.h"

class JsonResponsePacketSerializer
{
private:
	static void getSizeIntoBuffer(int size, Buffer& buffer);


public:

	static Buffer serializeResponse(ErrResponse& res);
	static Buffer serializeResponse(LoginResponse& res);
	static Buffer serializeResponse(SignUpResponse& res);
	static Buffer serializeResponse(RemoveUserResponse& res);
};