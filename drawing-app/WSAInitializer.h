#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "Pch.h"


class WSAInitializer
{
public:
	WSAInitializer();
	~WSAInitializer();
};