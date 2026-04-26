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
	LoginRequestHandler(LoginManager& loginManager, RequestHandlerFactory& handlerFactory);

	virtual bool isRequestRelevant(RequestInfo& info) override;
	virtual RequestResult handlerRequest(RequestInfo& info) override;

private:
	RequestResult Login(RequestInfo& info);
	RequestResult SignUp(RequestInfo& info);
	RequestResult Remove(RequestInfo& info);

	LoginManager& m_loginManager;
	RequestHandlerFactory& m_handlerFactory;
};