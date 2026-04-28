#pragma once
#include "pch.h"
#include "sqliteDataBase.h"
#include "LoggedUser.h";

enum class LoginStatus : unsigned int
{
	LOGIN_SUCCESS,
	LOGIN_FAILED,
	USER_ALREADY_LOGGED_IN
};

enum class SignUpStatus : unsigned int
{
	SIGNUP_SUCCESS,
	USER_ALREADY_EXISTS,
	SIGNUP_FAILED
};

enum class RemoveStatus : unsigned int
{
	REMOVE_SUCCESS,
	USER_NOT_FOUND,
	REMOVE_FAILED
};

class LoginManager
{
public:
	LoginManager(IDatabase* m_database);
	LoginStatus login(const std::string& name, const std::string& pass);
	SignUpStatus signup(const std::string& name, const std::string& pass, const std::string& mail);
	void logout(const std::string& name);
	RemoveStatus Remove(const std::string& name);

private:
	bool isUserLoggedIn(const std::string& name) const;
	void privateLogout(const std::string& name);

	mutable std::mutex _loggedUser_mutex;
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
};
