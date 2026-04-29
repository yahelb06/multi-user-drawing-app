#include "Room.h"

bool Room::doesRoomEmpty() const
{
    return (this->_userInTheRoom.empty());
}

std::string Room::MakeRandomRoomId()
{
    int i = 0;
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&*";
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);
    std::string random_string;
    random_string.reserve(ROOM_ID_LENGTH);
    for (i = 0; i < ROOM_ID_LENGTH; ++i) {
        random_string += characters[distribution(generator)];
    }

    return random_string;
}

Room::Room(const LoggedUser& manager)
{
    this->_id = MakeRandomRoomId();
    this->_userInTheRoom.push_back(manager);
}

Room::Room(const LoggedUser& manager, const Paint& paint)
{
    this->_id = MakeRandomRoomId();
    this->_userInTheRoom.push_back(manager);
    this->_paint = paint;
}

std::string Room::GetRoomId() const
{
    return this->_id;
}

LoggedUser Room::GetRoomManager() const
{
    return this->_userInTheRoom[0];
}

void Room::setPaint(const Paint& paint)
{
    this->_paint = paint;
}

void Room::setUserToWaitingRoom(const LoggedUser& user)
{
    std::lock_guard<std::mutex> lock(this->m_waitingRoom_mutex);
    this->_userWantToJoin.push_back(user);
}

void Room::removeUserFromWaitingRoom(const LoggedUser& user)
{
    this->_userWantToJoin.erase(std::remove(_userWantToJoin.begin(), _userWantToJoin.end(), user), _userWantToJoin.end());
}

bool Room::isUserManager(const LoggedUser& manager)
{
    if (this->_userInTheRoom.empty())
    {
        return false;
    }
    return (this->_userInTheRoom[0] == manager);
}

void Room::stopJoinRequest(const LoggedUser& user)
{
    std::lock_guard<std::mutex> lock(this->m_waitingRoom_mutex);
    removeUserFromWaitingRoom(user);
}

void Room::CloseRoom()
{
    std::lock_guard<std::mutex> lock(this->m_UserInRoom_mutex);
    this->_userInTheRoom.clear();
}

bool Room::addUserToRoom(const LoggedUser& manager, const LoggedUser& userToAdd, const bool& accept)
{

    std::lock_guard<std::mutex> waitingRoomLock(this->m_waitingRoom_mutex);
    std::lock_guard<std::mutex> lock(this->m_UserInRoom_mutex);
    if (isUserManager(manager))
    {
        if (accept)
        {
            this->_userInTheRoom.push_back(userToAdd);
            return true;
        }
        removeUserFromWaitingRoom(userToAdd);
    }
    return false;
}

bool Room::exitRoom(const LoggedUser& user)
{
    std::lock_guard<std::mutex> lock(this->m_UserInRoom_mutex);
    this->_userInTheRoom.erase(std::remove(_userInTheRoom.begin(), _userInTheRoom.end(), user), _userInTheRoom.end());
    if (this->_userInTheRoom.empty())
    {
        CloseRoom();
        return true;
    }
    return false;
}

bool Room::removeUser(const LoggedUser& manager, const LoggedUser& userToRemove)
{
    std::lock_guard<std::mutex> lock(this->m_UserInRoom_mutex);
    if (isUserManager(manager))
    {
        this->_userInTheRoom.erase(std::remove(_userInTheRoom.begin(), _userInTheRoom.end(), userToRemove), _userInTheRoom.end());
        return true;
    }
    return false;
}
