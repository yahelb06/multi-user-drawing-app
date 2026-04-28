#pragma once
#include "Pch.h"
#include "LoggedUser.h"
#include "Paint.h"

class Room
{
private:

	std::string _id;
	std::vector<LoggedUser> _userInTheRoom;
	Paint _paint;

	bool doesRoomEmpty() const;
	static std::string MakeRandomRoomId();

public:
	Room(const std::string& id, const LoggedUser& manager, const Paint& paint);

	std::string GetRoomId() const;

	Room CreateRoom(const LoggedUser& manager, const Paint& paint) const;
	void CloseRoom();
	void addUserToRoom(const LoggedUser& user);
};