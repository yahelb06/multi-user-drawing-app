#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database) : m_database(database), m_loginManager(database)
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
