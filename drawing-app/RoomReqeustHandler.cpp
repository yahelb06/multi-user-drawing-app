#include "RoomReqeustHandler.h"
#include "RequestHandlerFactory.h"
#include "Server.h"

RoomRequestHandler::RoomRequestHandler(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)
{
}

RoomRequestHandler::~RoomRequestHandler()
{
}

bool RoomRequestHandler::isRequestRelevant(RequestInfo& info)
{
	MessageCode code = static_cast<MessageCode>(info.id);

	return (code == MessageCode::ADD_USER_TO_ROOM ||
		code == MessageCode::REMOVE_PAINT_FROM_ROOM ||
		code == MessageCode::ACCEPT_USER ||
		code == MessageCode::REMOVE_USER ||
		code == MessageCode::ADD_LINE_TO_PAINT ||
		code == MessageCode::GET_PAINT_FROM_ROOM);
}

RequestResult RoomRequestHandler::handlerRequest(RequestInfo& info)
{
	MessageCode code = static_cast<MessageCode>(info.id);
	if (!isRequestRelevant(info))
	{
		RequestResult res;
		ErrResponse err;
		err.message = "Request failed, Illegal message code.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
	}
	else if (code == MessageCode::ADD_USER_TO_ROOM)
	{
		return AddUser(info);
	}
	else if (code == MessageCode::REMOVE_PAINT_FROM_ROOM)
	{
		return RemovePaintFromRoom(info);
	}
	else if (code == MessageCode::ACCEPT_USER)
	{
		return AcceptUser(info);
	}
	else if (code == MessageCode::REMOVE_USER)
	{
		return RemoveUserFromRoom(info);
	}
	else if (code == MessageCode::ADD_LINE_TO_PAINT)
	{
		return AddLineToPaint(info);
	}
	else if (code == MessageCode::GET_PAINT_FROM_ROOM)
	{
		return GetPaintFromRoom(info);
	}
}


RequestResult RoomRequestHandler::AddUser(const RequestInfo& info)
{
	bool showErr = false;
	RequestResult res;

	AddUserRequest req = JsonRequestPacketDeserializer::deserializeAddUserRequest(info.buffer);
	LoggedUser manager(req.manager, info.socket);
	SOCKET userToAddSocket = this->m_handlerFactory.getLoginManager().getSocketByUsername(req.userToAdd);
	LoggedUser userToAdd(req.userToAdd, userToAddSocket);
	AddUserStatus status = static_cast<AddUserStatus>(this->m_handlerFactory.getRoomManager().AddUser(manager, userToAdd, req.roomId, req.accept));

	if (status == AddUserStatus::ADD_SUCCESS)
	{
		AddUserResponse addUser;
		addUser.status = static_cast<unsigned int>(AddUserStatus::ADD_SUCCESS);
		res.response = JsonResponsePacketSerializer::serializeResponse(addUser);
		res.newHandler = this;

		AcceptUserResponse accept;
		accept.status = 1;
		accept.roomId = req.roomId;
		accept.usersInRoom = this->m_handlerFactory.getRoomManager().getUsersInRoom(accept.roomId);
		Buffer userAddRes = JsonResponsePacketSerializer::serializeResponse(accept);
		if (!sendMsgToSingleUser(userAddRes, req.roomId, userToAdd, this->m_handlerFactory.CreateRoomRequest()))
		{
			showErr = true;
		}
	}
	else if (status == AddUserStatus::USER_ISNT_THE_MANAGER || showErr)
	{
		ErrResponse err;
		err.message = err.message = "Add failed, user is not the manager. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
	}
	else
	{
		ErrResponse err;
		err.message = err.message = "Add failed. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
	}
	return res;
}

RequestResult RoomRequestHandler::AcceptUser(const RequestInfo& info)
{
	RequestResult res;

	AddUserRequest req = JsonRequestPacketDeserializer::deserializeAddUserRequest(info.buffer);

	if (req.accept)
	{
		AcceptUserResponse acceptUser;
		acceptUser.status = 1;
		acceptUser.roomId = req.roomId;
		acceptUser.usersInRoom = this->m_handlerFactory.getRoomManager().getUsersInRoom(acceptUser.roomId);

		res.response = JsonResponsePacketSerializer::serializeResponse(acceptUser);
		res.newHandler = this;
		return res;
	}
}

RequestResult RoomRequestHandler::RemoveUserFromRoom(const RequestInfo& info)
{
	bool showErr = false;
	RequestResult res;

	RemoveUserFromRoomRequest req = JsonRequestPacketDeserializer::deserializeRemoveUserFromRoomRequest(info.buffer);
	LoggedUser manager(req.manager, info.socket);
	SOCKET userToRemoveSocket = this->m_handlerFactory.getLoginManager().getSocketByUsername(req.userToRemove);
	LoggedUser userToRemove(req.userToRemove, userToRemoveSocket);
	RoomLogOutStatus status = static_cast<RoomLogOutStatus>(this->m_handlerFactory.getRoomManager().RemoveUserFromRoom(manager, userToRemove, req.roomId));

	if (status == RoomLogOutStatus::LOG_OUT_SUCCESS)
	{
		RemoveUserFromRoomResponse removeUser;
		removeUser.status = static_cast<unsigned int>(status);
		res.response = JsonResponsePacketSerializer::serializeResponse(removeUser);
		res.newHandler = this;

		RoomLogOutResponse logOut;
		logOut.status = 1;
		Buffer userRemoveRes = JsonResponsePacketSerializer::serializeResponse(logOut);
		if (!sendMsgToSingleUser(userRemoveRes, req.roomId, userToRemove, this->m_handlerFactory.createMenuRequest()))
		{
			showErr = true;
		}
	}
	else if (status == RoomLogOutStatus::LOG_OUT_FAILED || showErr)
	{
		ErrResponse err;
		err.message = err.message = "Remove failed, User not manager. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this->m_handlerFactory.createMenuRequest();
	}
	else
	{
		ErrResponse err;
		err.message = err.message = "Remove failed. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this->m_handlerFactory.createMenuRequest();
	}
	return res;
}

RequestResult RoomRequestHandler::RemovePaintFromRoom(const RequestInfo& info)
{
	bool showErr = false;
	RequestResult res;

	RemovePaintFromRoomRequest req = JsonRequestPacketDeserializer::deserializeRemovePaintFromRoomRequest(info.buffer);
	LoggedUser manager(req.data.manager, info.socket);
	PaintRoomStatus status = static_cast<PaintRoomStatus>(this->m_handlerFactory.getRoomManager().RemovePaint(manager, req.data.roomId, req.data.paintName));

	if (status == PaintRoomStatus::SUCCESS)
	{
		RemovePaintFromRoomResponse removePaint;
		removePaint.status = static_cast<unsigned int>(status);
		res.response = JsonResponsePacketSerializer::serializeResponse(removePaint);
		res.newHandler = this;

		if (!SendMsgToAllUsersInRoom(res.response, req.data.roomId, this))
		{
			showErr = true;
		}
	}
	else if (status == PaintRoomStatus::FAILED || showErr)
	{
		ErrResponse err;
		err.message = "Remove paint failed, Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
	}
	else
	{
		ErrResponse err;
		err.message = "Room not found remove paint failed, Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
	}
	return res;
}

RequestResult RoomRequestHandler::AddLineToPaint(const RequestInfo& info)
{
	bool showErr = false;
	RequestResult res;

	AddLineToPaintRequest req = JsonRequestPacketDeserializer::deserialAddLineToPaintRequest(info.buffer);

	AddLinesToPaintStatus status = static_cast<AddLinesToPaintStatus>(this->m_handlerFactory.getRoomManager().AddLinesToPaint(req.roomId, req.manager, req.linesToAdd));
	if (status == AddLinesToPaintStatus::SUCCESS)
	{

		AddLinesToPaintResponse addLines;
		addLines.status = static_cast<unsigned int>(status);
		res.response = JsonResponsePacketSerializer::serializeResponse(addLines);
		res.newHandler = this;

		GetPaintFromRoomResponse newLines;
		newLines.vecLines = req.linesToAdd;
		Buffer otherPlayerRes = JsonResponsePacketSerializer::serializeResponse(newLines);
		if (!SendMsgToAllUsersInRoom(otherPlayerRes, req.roomId, this))
		{
			showErr = true;
		}
	}
	else if (status == AddLinesToPaintStatus::FAILED || showErr)
	{
		ErrResponse err;
		err.message = "Add line failed, Try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
	}
	else if (status == AddLinesToPaintStatus::ROOM_NOT_FOUND)
	{
		ErrResponse err;
		err.message = "Room did not found, Try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
	}
	return res;
}

RequestResult RoomRequestHandler::GetPaintFromRoom(const RequestInfo& info)
{
	RequestResult res;

	try
	{
		GetPaintFromRoomRequest req = JsonRequestPacketDeserializer::deserialGetPaintFromRoomRequest(info.buffer);

		GetPaintFromRoomResponse paintLines;
		paintLines.vecLines = this->m_handlerFactory.getRoomManager().GetPaintFromRoom(req.roomId);

		res.response = JsonResponsePacketSerializer::serializeResponse(paintLines);
		res.newHandler = this;
		return res;
	}
	catch (...)
	{
		ErrResponse err;
		err.message = "Error happend, Try again";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this;
		return res;
	}
}

bool RoomRequestHandler::sendMsgToSingleUser(const Buffer& res, const std::string& roomId, const LoggedUser& user, IRequestHandler* handler)
{
	try
	{
		SOCKET playerSocket = this->m_handlerFactory.getLoginManager().getSocketByUsername(user.getUserName());
		if (playerSocket == INVALID_SOCKET)
		{
			return false;
		}
		this->m_handlerFactory.getServer()->updateClientHandler(playerSocket, handler);
		send(playerSocket, reinterpret_cast<const char*>(res.data()), res.size(), 0);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool RoomRequestHandler::SendMsgToAllUsersInRoom(const Buffer& res, const std::string& roomId, IRequestHandler* handler)
{
	try
	{
		std::vector<std::string> playerInRoom = this->m_handlerFactory.getRoomManager().getUsersInRoom(roomId);
		for (const auto& player : playerInRoom)
		{
			SOCKET playerSocket = this->m_handlerFactory.getLoginManager().getSocketByUsername(player);
			if (playerSocket == INVALID_SOCKET)
			{
				return false;
			}
			this->m_handlerFactory.getServer()->updateClientHandler(playerSocket, handler);
			send(playerSocket, reinterpret_cast<const char*>(res.data()), res.size(), 0);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}
