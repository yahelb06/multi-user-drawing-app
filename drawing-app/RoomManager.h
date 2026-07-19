#pragma once
#include "Pch.h"
#include "IDatabase.h"
#include "Room.h"
#include "Paint.h"

enum class JoinRoomStatus : unsigned int
{
	WAITING_FOR_MANAGER = 1,
	JOIN_FAILED
};

enum class AddUserStatus : unsigned int
{
	ADD_SUCCESS = 1,
	USER_ISNT_THE_MANAGER,
	ROOM_NOT_FOUND
};

enum class RoomLogOutStatus : unsigned int
{
	LOG_OUT_SUCCESS = 1,
	LOG_OUT_FAILED,
	ROOM_CLOSED
};

enum class PaintRoomStatus : unsigned int
{
	SUCCESS = 1,
	FAILED,
	ROOM_NOT_FOUND
};

enum class AddLinesToPaintStatus : unsigned int
{
	SUCCESS = 1,
	FAILED,
	ROOM_NOT_FOUND,
};

enum class SavePaintStatus : unsigned int
{
	SUCCESS = 1,
	FAILED,
	ROOM_NOT_FOUND,
};


class RoomManager
{
private:

	IDatabase* m_database;
	std::vector<Room> m_RoomOpen;
	mutable std::mutex m_roomManager_mutex;



public:
	RoomManager(IDatabase* database);

	std::vector<Room>::iterator FindRoom(const std::string& roomId);

	std::string CreateRoom(const LoggedUser& user);
	JoinRoomStatus JoinRoom(const LoggedUser& user, const std::string& roomId);
	RoomLogOutStatus LogOut(const LoggedUser& user, const std::string& roomId);
	AddUserStatus AddUser(const LoggedUser& manager, const LoggedUser& userToAdd, const std::string& roomId, const bool& accept);
	RoomLogOutStatus RemoveUserFromRoom(const LoggedUser& manager, const LoggedUser& userToRemove, const std::string& roomId);

	PaintRoomStatus RemovePaint(const LoggedUser& manager, const std::string& roomId, const std::string& paintName);
	PaintRoomStatus UploadPaint(const LoggedUser& manager, const std::string& roomId, const Paint& paint);
	std::vector<Room>& getRooms();
	std::vector<std::string> getUsersInRoom(const std::string& roomId);
	std::vector<std::string> getUserPaintsName(const std::string name) const;
	Paint GetPaint(const std::string& username, const std::string& paintName);

	AddLinesToPaintStatus AddLinesToPaint(const std::string& roomId, const std::string& manager, const std::vector<Line>& linesToAdd);
	SavePaintStatus SavePaint(const std::string& roomId, const std::string manager, const std::string& paintName);


	std::vector<Line> GetPaintFromRoom(const std::string& roomId);
};