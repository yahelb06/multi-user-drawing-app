#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(IDatabase* database) : m_database(database), m_loginManager(database)
{
}

LoginRequestHandler* RequestHandlerFactory::CreateLoginRequest()
{
	return new LoginRequestHandler(m_loginManager, *this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}
