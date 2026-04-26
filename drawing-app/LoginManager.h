#pragma once
#include "pch.h"
//#include "sqliteDataBase.h"
#include "LoggedUser.h"

enum LoginStatus
{
	LOGIN_SUCCESS,
	LOGIN_FAILED,
	USER_ALREADY_LOGGED_IN
};

enum SignUpStatus
{
	SIGNUP_SUCCESS,
	USER_ALREADY_EXISTS,
	SIGNUP_FAILED
};

class LoginManager
{
public:
	//LoginManager(IDatabase* m_database);
	LoginStatus login(const std::string& name, const std::string& pass);
	SignUpStatus signup(const std::string& name, const std::string& pass, const std::string& mail);
	void logout(std::string& name);

private:
	bool isUserLoggedIn(const std::string& name) const;
	//IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
};
