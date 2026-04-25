#ifndef PCH_H
#define PCH_H

enum class MessageCode : char
{
	LOGIN_REQUEST = 100,
	SIGNUP_REQUEST = 110,
};

#define PORT 8826
#define BUFFER_SIZE 1024

#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <mutex>
#include <thread>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <exception>

typedef std::vector<unsigned char> Buffer;


#endif