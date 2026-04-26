#pragma once
#include "Pch.h"
#include "IDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"

class RequestHandlerFactory
{
public:

	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler* CreateLoginRequest();
	LoginManager& getLoginManager();

private:
	IDatabase* m_database;
	LoginManager m_loginManager;
};