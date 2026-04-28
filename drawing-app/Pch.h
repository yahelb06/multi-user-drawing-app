#ifndef PCH_H
#define PCH_H

enum class MessageCode : unsigned char
{
	ERROR_CODE = 90,
	LOGIN_REQUEST = 100,
	SIGNUP_REQUEST = 110,
	REMOVE_USER = 120
};

#define PORT 8826
#define BUFFER_SIZE 1024
#define DB_FILE_NAME "DB.sqlite"
#define START_OF_DATA 5

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
#include <io.h>
#include <algorithm>

typedef std::vector<unsigned char> Buffer;


#endif