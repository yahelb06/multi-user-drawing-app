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

Room::Room(const LoggedUser& manager) : _id(MakeRandomRoomId()), _paint(NO_PAINT_IN_ROOM)
{
    this->_userInTheRoom.push_back(manager);
}

Room::Room(const LoggedUser& manager, const Paint& paint) : _id(MakeRandomRoomId()), _paint(paint)
{
    this->_userInTheRoom.push_back(manager);
}

std::string Room::GetRoomId() const
{
    return this->_id;
}

LoggedUser Room::GetRoomManager() const
{
    return this->_userInTheRoom[0];
}

std::vector<std::string> Room::getUserInRoom() const
{
    std::lock_guard<std::mutex> lock(this->m_UserInRoom_mutex);
    std::vector<std::string> usernamesVec;
    for (const auto& username : this->_userInTheRoom)
    {

        std::string name = username.getUserName();
        usernamesVec.push_back(name);
    }
    return usernamesVec;
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

bool Room::doesHavePaint()
{
    return (this->_paint.getPaintName() == NO_PAINT_IN_ROOM);
}

bool Room::doesCurrentPaint(const std::string& paintName)
{
    return (this->_paint.getPaintName() == paintName);
}

void Room::stopJoinRequest(const LoggedUser& user)
{
    std::lock_guard<std::mutex> lock(this->m_waitingRoom_mutex);
    removeUserFromWaitingRoom(user);
}

bool Room::removePaint(const LoggedUser& manager, const std::string& paintName)
{
    std::lock_guard<std::mutex> lock(this->m_UserInRoom_mutex);
    if (!this->isUserManager(manager))
    {
        return false;
    }
    if (this->doesCurrentPaint(paintName))
    {
        this->_paint.setPaintName("no_paint");
        this->_paint.cleanPaint();
        return true;
    }
    return false;
}

bool Room::addPaint(const LoggedUser& manager, const std::string& paintName, const std::vector<Line>& LinesInPaint)
{
    std::lock_guard<std::mutex> lock(this->m_UserInRoom_mutex);
    if (!this->isUserManager(manager))
    {
        return false;
    }
    if (!doesHavePaint())
    {
        return false;
    }
    this->_paint.setPaintName(paintName);
    this->_paint.setPaintLines(LinesInPaint);
    return true;
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
