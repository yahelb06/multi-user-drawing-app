#pragma once
#include "Pch.h"
#include "IDatabase.h"
#include "Room.h"

enum class AddUserStatus : unsigned int
{
	ADD_SUCCESS = 1,
	USER_ISNT_THE_MANAGER
};

class RoomManager
{
private:

	IDatabase* m_database;
	std::vector<Room> m_RoomOpen;
	mutable std::mutex m_roomManager_mutex;


public:
	RoomManager(IDatabase* database);

	void CreateRoom(const LoggedUser& user);
};