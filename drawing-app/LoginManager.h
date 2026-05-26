#pragma once
#include "pch.h"
#include "sqliteDataBase.h"
#include "LoggedUser.h";

enum class LoginStatus : unsigned int
{
	LOGIN_SUCCESS = 1,
	LOGIN_FAILED,
	USER_ALREADY_LOGGED_IN
};

enum class SignUpStatus : unsigned int
{
	SIGNUP_SUCCESS = 1,
	USER_ALREADY_EXISTS,
	SIGNUP_FAILED
};

enum class UserLogOutStatus : unsigned int
{
	LOG_OUT_SUCCESS = 1,
	LOG_OUT_FAILED
};

enum class RemoveStatus : unsigned int
{
	REMOVE_SUCCESS = 1,
	USER_NOT_FOUND,
	REMOVE_FAILED
};

class LoginManager
{
public:
	LoginManager(IDatabase* m_database);
	LoginStatus login(const std::string& name, const std::string& pass, SOCKET clientSocket);
	SignUpStatus signup(const std::string& name, const std::string& pass, const std::string& mail, SOCKET clientSocket);
	UserLogOutStatus logout(const std::string& name, SOCKET clientSocket);
	RemoveStatus Remove(const std::string& name, SOCKET clientSocket);

	SOCKET getSocketByUsername(const std::string& name) const;

private:
	bool isUserLoggedIn(const std::string& name);
	UserLogOutStatus privateLogout(const std::string& name, SOCKET clientSocket);

	mutable std::mutex _loggedUser_mutex;
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
};
