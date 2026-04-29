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
		code == MessageCode::ADD_USER_TO_ROOM ||
		code == MessageCode::REMOVE_USER_FROM_ROOM);
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
	else if (code == MessageCode::ADD_USER_TO_ROOM)
	{
		return AddUser(info);
	}
	else if (code == MessageCode::REMOVE_USER_FROM_ROOM)
	{
		return RemoveUserFromRoom(info);
	}
}

RequestResult MenuRequestHandler::Logout(const RequestInfo& info)
{
	RequestResult res;
	RoomLogOutStatus status;

	RoomLogOutRequest req = JsonRequestPacketDeserializer::deserializeRoomLogOutRequest(info.buffer);
	status = static_cast<RoomLogOutStatus>(this->m_handlerFactory.getRoomManager().LogOut(req.username, req.roomId));

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
	LoggedUser user(req.username);
	this->m_handlerFactory.getRoomManager().CreateRoom(user);
	CreateRoomResponse status;
	status.status = 1;
	res.response = JsonResponsePacketSerializer::serializeResponse(status);
	res.newHandler = nullptr;
	//res.newHandler = room...
	return res;
}

RequestResult MenuRequestHandler::JoinRoom(const RequestInfo& info)
{
	RequestResult res;
	
	JoinRoomRequest req = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
	JoinRoomStatus status = static_cast<JoinRoomStatus>(this->m_handlerFactory.getRoomManager().JoinRoom(req.username, req.roomId));

	if (status == JoinRoomStatus::WAITING_FOR_MANAGER)
	{
		JoinRoomResponse joinRoom;
		joinRoom.status = static_cast<unsigned int>(JoinRoomStatus::WAITING_FOR_MANAGER);
		res.response = JsonResponsePacketSerializer::serializeResponse(joinRoom);
		res.newHandler = nullptr;
		//res.newHandler = room;
	}
	else
	{
		ErrResponse err;
		err.message = err.message = "Join room failed. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this->m_handlerFactory.createMenuRequest();
	}
	return res;
}

RequestResult MenuRequestHandler::AddUser(const RequestInfo& info)
{
	RequestResult res;

	AddUserRequest req = JsonRequestPacketDeserializer::deserializeAddUserRequest(info.buffer);
	AddUserStatus status = static_cast<AddUserStatus>(this->m_handlerFactory.getRoomManager().AddUser(req.manager, req.userToAdd, req.roomId, req.accept));

	if (status == AddUserStatus::ADD_SUCCESS)
	{
		AddUserResponse addUser;
		addUser.status = static_cast<unsigned int>(AddUserStatus::ADD_SUCCESS);
		res.response = JsonResponsePacketSerializer::serializeResponse(addUser);
		res.newHandler = nullptr;
		//res.newHandler = room...
	}
	else if (status == AddUserStatus::USER_ISNT_THE_MANAGER)
	{
		ErrResponse err;
		err.message = err.message = "Add failed, user is not the manager. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this->m_handlerFactory.createMenuRequest();
	}
	else
	{
		ErrResponse err;
		err.message = err.message = "Add failed. Please try again.";
		res.response = JsonResponsePacketSerializer::serializeResponse(err);
		res.newHandler = this->m_handlerFactory.createMenuRequest();
	}
	return res;
}

RequestResult MenuRequestHandler::RemoveUserFromRoom(const RequestInfo& info)
{
	RequestResult res;

	RemoveUserFromRoomRequest req = JsonRequestPacketDeserializer::deserializeRemoveUserFromRoomRequest(info.buffer);
	RoomLogOutStatus status = static_cast<RoomLogOutStatus>(this->m_handlerFactory.getRoomManager().RemoveUserFromRoom(req.manager, req.userToRemove, req.roomId));

	if (status == RoomLogOutStatus::LOG_OUT_SUCCESS)
	{
		RemoveUserFromRoomResponse removeUser;
		removeUser.status = static_cast<unsigned int>(status);
		res.response = JsonResponsePacketSerializer::serializeResponse(removeUser);
		res.newHandler = this->m_handlerFactory.createMenuRequest();
	}
	else if (status == RoomLogOutStatus::LOG_OUT_FAILED)
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
}
