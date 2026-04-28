#include "RoomManager.h"

RoomManager::RoomManager(IDatabase* database) : m_database(database)
{
}

void RoomManager::CreateRoom(const LoggedUser& user)
{
    std::lock_guard<std::mutex> lock(this->m_roomManager_mutex);
    Room userRoom(user);
    this->m_RoomOpen.push_back(userRoom);
}
