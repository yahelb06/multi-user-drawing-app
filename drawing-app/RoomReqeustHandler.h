#pragma once
#include "Pch.h"
#include "IRequestHandler.h"
#include "Responses.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "RoomManager.h"

class Server;
class RequestHandlerFactory;

class RoomRequestHandler : public IRequestHandler
{
public:
	RoomRequestHandler(RequestHandlerFactory& handlerFactory);
	~RoomRequestHandler();

	virtual bool isRequestRelevant(RequestInfo& info) override;
	virtual RequestResult handlerRequest(RequestInfo& info) override;

private:

	RequestResult AddUser(const RequestInfo& info);
	RequestResult AcceptUser(const RequestInfo& info);
	RequestResult RemoveUserFromRoom(const RequestInfo& info);
	RequestResult RemovePaintFromRoom(const RequestInfo& info);
	RequestResult AddLineToPaint(const RequestInfo& info);
	RequestResult GetUserPaintsName(const RequestInfo& info);
	RequestResult GetPaintByName(const RequestInfo& info);
	RequestResult UploadPaintToRoom(const RequestInfo& info);
	RequestResult GetPaintFromRoom(const RequestInfo& info);
	RequestResult SavePaint(const RequestInfo& info);

	RequestHandlerFactory& m_handlerFactory;

	bool sendMsgToSingleUser(const Buffer& res, const std::string& roomId, const LoggedUser& user, IRequestHandler* handler);
	bool SendMsgToAllUsersInRoom(const Buffer& res, const std::string& roomId, IRequestHandler* handler);
};