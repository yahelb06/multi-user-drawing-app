#include "RoomManager.h"

RoomManager::RoomManager(IDatabase* database) : m_database(database)
{
}

std::vector<Room>::iterator RoomManager::FindRoom(const std::string& roomId)
{
    const auto& it = std::find_if(m_RoomOpen.begin(), m_RoomOpen.end(), [&](const Room& r)
        {
            return r.GetRoomId() == roomId;
        });
    return it;
}

std::string RoomManager::CreateRoom(const LoggedUser& user)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    this->m_RoomOpen.emplace_back(user);
    return this->m_RoomOpen.back().GetRoomId();
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

PaintRoomStatus RoomManager::RemovePaint(const LoggedUser& manager, const std::string& roomId, const std::string& paintName)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        //if the paint to remove is the paint right now
        if (it->doesCurrentPaint(paintName))
        {
            int paintId = this->m_database->getPaintId(manager.getUserName(), paintName);
            if (it->removePaint(manager, paintName))
            {
                return PaintRoomStatus::SUCCESS;
            }
        }
        return PaintRoomStatus::FAILED;
    }
    return PaintRoomStatus::ROOM_NOT_FOUND;
}

PaintRoomStatus RoomManager::UploadPaint(const LoggedUser& manager, const std::string& roomId, const Paint& paint)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        if (it->addPaint(manager, paint.getPaintName(), paint.getPaintLines()))
        {
            return PaintRoomStatus::SUCCESS;
        }
    }
    return PaintRoomStatus::ROOM_NOT_FOUND;
}

std::vector<Room>& RoomManager::getRooms()
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    return this->m_RoomOpen;
}

std::vector<std::string> RoomManager::getUsersInRoom(const std::string& roomId)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);
    return it->getUserInRoom();
}

std::vector<std::string> RoomManager::getUserPaintsName(const std::string name) const
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    return this->m_database->GetUserPaintsName(name);
}

Paint RoomManager::GetPaint(const std::string& username, const std::string& paintName)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    int paintId = this->m_database->getPaintId(username, paintName);
    return this->m_database->GetPaint(paintId, paintName);
}

AddLinesToPaintStatus RoomManager::AddLinesToPaint(const std::string& roomId, const std::string& manager, const std::vector<Line>& linesToAdd)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        if (it->AddLinesToPaint(linesToAdd))
        {
            return AddLinesToPaintStatus::SUCCESS;
        }
        return AddLinesToPaintStatus::FAILED;
    }
    return AddLinesToPaintStatus::ROOM_NOT_FOUND;
}

SavePaintStatus RoomManager::SavePaint(const std::string& roomId, const std::string manager, const std::string& paintName)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        if (manager == it->GetRoomManager().getUserName())
        {
            Paint currPaint = it->GetPaint();
            try
            {
                if (this->m_database->savePaint(manager, paintName, currPaint.getPaintLines()))
                {
                    return SavePaintStatus::SUCCESS;
                }
            }
            catch (...)
            {
                return SavePaintStatus::FAILED;
            }
            return SavePaintStatus::FAILED;
        }
    }
    return SavePaintStatus::ROOM_NOT_FOUND;
}

std::vector<Line> RoomManager::GetPaintFromRoom(const std::string& roomId)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    const auto& it = FindRoom(roomId);

    //found the room
    if (it != this->m_RoomOpen.end())
    {
        return it->GetPaint().getPaintLines();
    }
    throw std::runtime_error("Room not found");
}
