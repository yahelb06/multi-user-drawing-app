#pragma once
#include "Pch.h"
#include "LoggedUser.h"
#include "Paint.h"

class Room
{
private:

	std::string _id;
	std::vector<LoggedUser> _userInTheRoom;
	std::vector<LoggedUser> _userWantToJoin;
	Paint _paint;

	bool doesRoomEmpty() const;
	static std::string MakeRandomRoomId();

public:
	Room(const LoggedUser& manager);
	Room(const LoggedUser& manager, const Paint& paint);

	std::string GetRoomId() const;
	LoggedUser GetRoomManager() const;

	void setPaint(const Paint& paint);

	void CloseRoom();
	void addUserToRoom(const LoggedUser& user);
	void exitRoom(const LoggedUser& user);
	bool removeUser(const LoggedUser& manager, const LoggedUser& userToRemove);

};