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

	mutable std::mutex m_waitingRoom_mutex;

	mutable std::mutex m_UserInRoom_mutex;

	bool doesRoomEmpty() const;
	static std::string MakeRandomRoomId();
	bool isUserManager(const LoggedUser& manager);

	void CloseRoom();

public:
	Room(const LoggedUser& manager);
	Room(const LoggedUser& manager, const Paint& paint);

	bool doesHavePaint();
	bool doesCurrentPaint(const std::string& paintName);
	std::string GetRoomId() const;
	LoggedUser GetRoomManager() const;

	void setPaint(const Paint& paint);
	void setUserToWaitingRoom(const LoggedUser& user);
	void removeUserFromWaitingRoom(const LoggedUser& user);

	void stopJoinRequest(const LoggedUser& user);
	bool removePaint(const LoggedUser& manager, const std::string& paintName);
	bool addPaint(const LoggedUser& manager, const std::string& paintName, const std::map<Line, std::string>& LinesInPaint);
	
	bool addUserToRoom(const LoggedUser& manager, const LoggedUser& userToAdd, const bool& accept);
	bool exitRoom(const LoggedUser& user);
	bool removeUser(const LoggedUser& manager, const LoggedUser& userToRemove);

};