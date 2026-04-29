#ifndef PCH_H
#define PCH_H

enum class MessageCode : unsigned char
{
	ERROR_CODE = 90,
	LOGIN_REQUEST = 100,
	SIGNUP_REQUEST = 110,
	USER_LOG_OUT_REQUEST = 120,
	REMOVE_USER = 130,
	ROOM_LOG_OUT_REQUEST = 140,
	CREATE_ROOM = 150,
	JOIN_ROOM = 160,
	ADD_USER_TO_ROOM = 170,
	REMOVE_USER_FROM_ROOM = 180
};

#define PORT 8826
#define BUFFER_SIZE 1024
#define DB_FILE_NAME "DB.sqlite"
#define START_OF_DATA 5
#define ROOM_ID_LENGTH 6

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
#include <utility>
#include <random>

typedef std::vector<unsigned char> Buffer;


#endif