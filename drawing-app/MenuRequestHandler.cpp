#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)
{
}

MenuRequestHandler::~MenuRequestHandler()
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo& info)
{
	MessageCode code = static_cast<MessageCode>(info.id);

	return (code == MessageCode::LOG_OUT_REQUEST);
}

RequestResult MenuRequestHandler::handlerRequest(RequestInfo& info)
{
	RequestResult res;
	MessageCode code = static_cast<MessageCode>(info.id);
	if (!isRequestRelevant(info))
	{
		ErrResponse err;
		err.message = "Request failed, Illegal message code.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
	}
	else if (code == MessageCode::LOG_OUT_REQUEST)
	{
		
	}
}

RequestResult MenuRequestHandler::Logout(const RequestInfo& info)
{
	RequestResult res;
	LogOutResponse status;

	LogOutRequest req = JsonRequestPacketDeserializer::deserializeLogOutUserRequest(info.buffer);
	this->m_handlerFactory.getLoginManager().logout(req.username);
	status.status = 1;
	res.response = JsonResponsePacketSerializer::serializeResponse(status);
	res.newHandler = m_handlerFactory.CreateLoginRequest();
	return res;
}
