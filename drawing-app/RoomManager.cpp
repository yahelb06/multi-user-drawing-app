#include "RoomManager.h"

RoomManager::RoomManager(IDatabase* database) : m_database(database)
{
}

std::vector<Room>::iterator RoomManager::FindRoom(const std::string& roomId) const
{
    const auto& it = std::find_if(m_RoomOpen.begin(), m_RoomOpen.end(), [&](const Room& r)
        {
            return r.GetRoomId() == roomId;
        });
}

std::string RoomManager::CreateRoom(const LoggedUser& user)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    Room userRoom(user);
    this->m_RoomOpen.push_back(userRoom);
    return userRoom.GetRoomId();
}

JoinRoomStatus RoomManager::JoinRoom(const LoggedUser& user, const std::string& roomId)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room id
    if (it != this->m_RoomOpen.end())
    {
        it->setUserToWaitingRoom(user);
        return JoinRoomStatus::WAITING_FOR_MANAGER;
    }
    return JoinRoomStatus::JOIN_FAILED;
}

RoomLogOutStatus RoomManager::LogOut(const LoggedUser& user, const std::string& roomId)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        if (it->exitRoom(user))
        {
            this->m_RoomOpen.erase(it);
            return RoomLogOutStatus::ROOM_CLOSED;
        }
        return RoomLogOutStatus::LOG_OUT_SUCCESS;
    }
    return RoomLogOutStatus::LOG_OUT_FAILED;
}

AddUserStatus RoomManager::AddUser(const LoggedUser& manager, const LoggedUser& userToAdd, const std::string& roomId, const bool& accept)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        if (it->addUserToRoom(manager, userToAdd, accept))
        {
            return AddUserStatus::ADD_SUCCESS;
        }
        return AddUserStatus::USER_ISNT_THE_MANAGER;
    }
    return AddUserStatus::ROOM_NOT_FOUND;
}

RoomLogOutStatus RoomManager::RemoveUserFromRoom(const LoggedUser& manager, const LoggedUser& userToRemove, const std::string& roomId)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        if (it->removeUser(manager, userToRemove))
        {
            return RoomLogOutStatus::LOG_OUT_SUCCESS;
        }
        return RoomLogOutStatus::LOG_OUT_FAILED;
    }
    return RoomLogOutStatus::ROOM_CLOSED;
}

PaintRoomStatus RoomManager::RemovePaint(const std::string& manager, const std::string& roomId, const std::string& paintName)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        //if the paint to remove is the paint right now
        if (it->doesCurrentPaint(paintName))
        {
            int paintId = this->m_database->getPaintId(manager, paintName);
            if (it->removePaint(manager, paintName))
            {
                return PaintRoomStatus::SUCCESS;
            }
        }
        return PaintRoomStatus::FAILED;
    }
    return PaintRoomStatus::ROOM_NOT_FOUND;
}

PaintRoomStatus RoomManager::AddPaint(const std::string& manager, const std::string& roomId, const std::string& paintName, const std::vector<Line>& LinesInPaint)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        //if there is paint right now in the room
        if (!it->doesHavePaint())
        {
            if (it->addPaint(manager, paintName, LinesInPaint))
            {
                return PaintRoomStatus::SUCCESS;
            }
        }
        return PaintRoomStatus::FAILED;
    }
    return PaintRoomStatus::ROOM_NOT_FOUND;
}

std::vector<Room> RoomManager::getRooms() const
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    return this->m_RoomOpen;
}

std::vector<std::string> RoomManager::getUsersInRoom(const std::string& roomId) const
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);
    return it->getUserInRoom();
}
