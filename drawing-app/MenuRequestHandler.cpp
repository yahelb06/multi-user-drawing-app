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

	return (code == MessageCode::ROOM_LOG_OUT_REQUEST ||
		code == MessageCode::CREATE_ROOM ||
		code == MessageCode::JOIN_ROOM ||
		code == MessageCode::GET_USER_PAINTS ||
		code == MessageCode::UPLOAD_PAINT_TO_ROOM);
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
		res.newHandler = this->m_handlerFactory.createMenuRequest();
		return res;
	}
	else if (code == MessageCode::ROOM_LOG_OUT_REQUEST)
	{
		return Logout(info);
	}
	else if (code == MessageCode::CREATE_ROOM)
	{
		return CreateRoom(info);
	}
	else if (code == MessageCode::JOIN_ROOM)
	{
		return JoinRoom(info);
	}
	else if (code == MessageCode::GET_USER_PAINTS)
	{
		return GetUserPaintsName(info);
	}
	else if (code == MessageCode::UPLOAD_PAINT_TO_ROOM)
	{
		return UploadPaintToRoom(info);
	}
	RequestResult res;
	res.newHandler = this;
	return res;
}

RequestResult MenuRequestHandler::Logout(const RequestInfo& info)
{
	RequestResult res;
	RoomLogOutStatus status;

	RoomLogOutRequest req = JsonRequestPacketDeserializer::deserializeRoomLogOutRequest(info.buffer);
	LoggedUser user(req.username, info.socket);
	status = static_cast<RoomLogOutStatus>(this->m_handlerFactory.getRoomManager().LogOut(user, req.roomId));

	if (status == RoomLogOutStatus::LOG_OUT_SUCCESS)
	{
		RoomLogOutResponse logOut;
		logOut.status = static_cast<unsigned int>(RoomLogOutStatus::LOG_OUT_SUCCESS);
		res.response = JsonResponsePacketSerializer::serializeResponse(logOut);
		res.newHandler = m_handlerFactory.CreateLoginRequest();
	}
	else
	{
		ErrResponse err;
		err.message = "Log out failed. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this->m_handlerFactory.createMenuRequest();
	}

	return res;
}

RequestResult MenuRequestHandler::CreateRoom(const RequestInfo& info)
{
	RequestResult res;

	CreateRoomRequest req = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);
	LoggedUser user(req.username, info.socket);
	std::string roomId = this->m_handlerFactory.getRoomManager().CreateRoom(user);
	CreateRoomResponse status;
	status.roomId = roomId;
	res.response = JsonResponsePacketSerializer::serializeResponse(status);
	res.newHandler = this->m_handlerFactory.CreateRoomRequest();
	return res;
}

RequestResult MenuRequestHandler::JoinRoom(const RequestInfo& info)
{
	RequestResult res;
	
	JoinRoomRequest req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
	LoggedUser user(req.username, info.socket);
	JoinRoomStatus status = static_cast<JoinRoomStatus>(this->m_handlerFactory.getRoomManager().JoinRoom(user, req.roomId));
	bool sendSucceed = sentJoinRoomToManager(this->m_handlerFactory.getRoomManager().FindRoom(req.roomId)->GetRoomManager().getSocket(), req.username);
	if (status == JoinRoomStatus::WAITING_FOR_MANAGER && sendSucceed)
	{
		JoinRoomResponse joinRoom;
		joinRoom.status = static_cast<unsigned int>(JoinRoomStatus::WAITING_FOR_MANAGER);
		res.response = JsonResponsePacketSerializer::serializeResponse(joinRoom);
		res.newHandler = this;
	}
	else
	{
		ErrResponse err;
		err.message = "Join room failed. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this->m_handlerFactory.createMenuRequest();
	}
	return res;
}

RequestResult MenuRequestHandler::UploadPaintToRoom(const RequestInfo& info)
{
	RequestResult res;

	UploadPaintToRoomRequest req = JsonRequestPacketDeserializer::deserializeUploadPaintToRoomRequest(info.buffer);

	Paint paint = (this->m_handlerFactory.getRoomManager().GetPaint(req.data.manager, req.data.paintName));

	LoggedUser manager(req.data.manager, info.socket);
	PaintRoomStatus status = static_cast<PaintRoomStatus>(this->m_handlerFactory.getRoomManager().UploadPaint(manager, req.data.roomId, paint));
	if (status == PaintRoomStatus::SUCCESS)
	{
		UploadPaintToRoomResponse addPaint;
		addPaint.paintLines = paint.getPaintLines();
		res.response = JsonResponsePacketSerializer::serializeResponse(addPaint);
		res.newHandler = this->m_handlerFactory.CreateRoomRequest();
	}
	else if (status == PaintRoomStatus::FAILED)
	{
		ErrResponse err;
		err.message = "Add paint failed, Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
	}
	else
	{
		ErrResponse err;
		err.message = "Room not found add paint failed, Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
	}
	return res;
}

RequestResult MenuRequestHandler::GetUserPaintsName(const RequestInfo& info)
{
	RequestResult res;
	try
	{
		GetUserPaintsNameRequest req = JsonRequestPacketDeserializer::deserializeGetUserPaintsRequest(info.buffer);

		GetUserPaintsNameResponse paintsName;
		paintsName.paintsName = this->m_handlerFactory.getRoomManager().getUserPaintsName(req.paintName);
		res.response = JsonResponsePacketSerializer::serializeResponse(paintsName);
		res.newHandler = this->m_handlerFactory.CreateRoomRequest();
		return res;
	}
	catch (...)
	{
		ErrResponse err;
		err.message = "Some error happend. Try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
		return res;
	}
}

bool MenuRequestHandler::sentJoinRoomToManager(SOCKET managerSocket, const std::string& userToAdd)
{
	if (managerSocket == INVALID_SOCKET)
	{
		std::cerr << "[Error] Manager socket is INVALID_SOCKET. Cannot send notification." << std::endl;
		return false;
	}
	Buffer buffer;
	buffer = JsonResponsePacketSerializer::serializeResponse(userToAdd);
	int bytesSent = send(managerSocket, reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0);
	if (bytesSent == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}
