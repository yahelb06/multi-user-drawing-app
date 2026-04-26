#pragma once
#include "Pch.h"
#include "IRequestHandler.h"
#include "Json.hpp"

class JsonRequestPacketDeserializer
{
public:

	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignUpRequest deserializeSignUpRequest(const Buffer& buffer);
	static RemoveUserRequest deserializeRemoveUserRequest(const Buffer& buffer);
};