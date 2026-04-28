#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)
{
}

MenuRequestHandler::~MenuRequestHandler()
{
}

bool MenuRequestHandler::isRequestRelevant(RequestInfo& info)
{
	MessageCode code = static_cast<MessageCode>(info.id);

	return (code == MessageCode::LOG_OUT_REQUEST || 
		code == MessageCode::CREATE_ROOM);
}

RequestResult MenuRequestHandler::handlerRequest(RequestInfo& info)
{
	MessageCode code = static_cast<MessageCode>(info.id);
	if (!isRequestRelevant(info))
	{
		RequestResult res;
		ErrResponse err;
		err.message = "Request failed, Illegal message code.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
	}
	else if (code == MessageCode::LOG_OUT_REQUEST)
	{
		return Logout(info);
	}
	else if (code == MessageCode::CREATE_ROOM)
	{
		return CreateRoom(info);
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

RequestResult MenuRequestHandler::CreateRoom(const RequestInfo& info)
{
	RequestResult res;

	CreateRoomRequest req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
	LoggedUser user(req.username);
	this->m_handlerFactory.getRoomManager().CreateRoom(user);
	CreateRoomResponse status;
	status.status = 1;
	res.response = JsonResponsePacketSerializer::serializeResponse(status);
	res.newHandler = nullptr;
	//res.newHandler = room...
	return res;
}
