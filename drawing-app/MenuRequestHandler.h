#pragma once
#include "Pch.h"
#include "IRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "RoomManager.h"

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(RequestHandlerFactory& handlerFactory);
	~MenuRequestHandler();

	virtual bool isRequestRelevant(RequestInfo& info) override;
	virtual RequestResult handlerRequest(RequestInfo& info) override;

private:
	RequestHandlerFactory& m_handlerFactory;

	RequestResult Logout(const RequestInfo& info);
	RequestResult CreateRoom(const RequestInfo& info);
	RequestResult JoinRoom(const RequestInfo& info);
	RequestResult AddUser(const RequestInfo& info);
	RequestResult AcceptUser(const RequestInfo& info);
	RequestResult RemoveUserFromRoom(const RequestInfo& info);
	RequestResult RemovePaintFromRoom(const RequestInfo& info);
	RequestResult AddPaintToRoom(const RequestInfo& info);
	RequestResult GetRooms(const RequestInfo& info);
};