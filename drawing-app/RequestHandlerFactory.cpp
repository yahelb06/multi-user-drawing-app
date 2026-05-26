#include "RequestHandlerFactory.h"
#include "Server.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database) : m_database(database), m_loginManager(database), m_RoomManager(database)
{
}

LoginRequestHandler* RequestHandlerFactory::CreateLoginRequest()
{
	return new LoginRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequest()
{
	return new MenuRequestHandler(*this);
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return this->m_RoomManager;
}

RoomRequestHandler* RequestHandlerFactory::CreateRoomRequest()
{
	return new RoomRequestHandler(*this);
}

Server* RequestHandlerFactory::getServer()
{
	return this->m_server;
}

void RequestHandlerFactory::SetServer(Server* server)
{
	this->m_server = server;
}
