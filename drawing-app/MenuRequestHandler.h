#pragma once
#include "Pch.h"
#include "IRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestHandlerFactory.h"

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
};