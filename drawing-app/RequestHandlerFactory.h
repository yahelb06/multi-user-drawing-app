#pragma once
#include "Pch.h"
#include "IDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomManager.h"
#include "RoomReqeustHandler.h"

class Server;

class RequestHandlerFactory
{
public:

	RequestHandlerFactory(IDatabase* database);
	LoginRequestHandler* CreateLoginRequest();
	LoginManager& getLoginManager();
	MenuRequestHandler* createMenuRequest();
	RoomManager& getRoomManager();
	RoomRequestHandler* CreateRoomRequest();
	Server* getServer();
	void SetServer(Server* server);

private:
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManager m_RoomManager;
	Server* m_server;
};