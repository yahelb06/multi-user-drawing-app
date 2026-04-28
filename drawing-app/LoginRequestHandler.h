#pragma once
#include "Pch.h"
#include "IRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "LoginManager.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory);
	~LoginRequestHandler();

	virtual bool isRequestRelevant(RequestInfo& info) override;
	virtual RequestResult handlerRequest(RequestInfo& info) override;

private:
	RequestResult Login(RequestInfo& info);
	RequestResult SignUp(RequestInfo& info);
	RequestResult Remove(RequestInfo& info);

	RequestHandlerFactory& m_handlerFactory;
};