#pragma once
#include "Pch.h"
#include "IDatabase.h"
#include "Room.h"

enum class CreateRoom : unsigned int
{
	CREATE_SUCCESS = 1,
	CREATE_FAILED
};

class RoomManager
{
private:

	IDatabase* m_database;
	std::vector<Room> m_RoomOpen;




};