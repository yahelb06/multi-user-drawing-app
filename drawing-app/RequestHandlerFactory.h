#pragma once
#include "Pch.h"
#include "IDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomManager.h"

class RequestHandlerFactory
{
public:

	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler* CreateLoginRequest();
	LoginManager& getLoginManager();
	MenuRequestHandler* createMenuRequest();
	RoomManager& getRoomManager();

private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManager m_RoomManager;
};