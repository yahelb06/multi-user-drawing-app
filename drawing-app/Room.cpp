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

Room::Room(const std::string& id, const LoggedUser& manager, const Paint& paint)
{
    this->_id = id;
    this->_userInTheRoom.push_back(manager);
    this->_paint = paint;
}

std::string Room::GetRoomId() const
{
    return this->_id;
}

Room Room::CreateRoom(const LoggedUser& manager, const Paint& paint) const
{
    std::string id = MakeRandomRoomId();
    return Room(id, manager, paint);
}

void Room::CloseRoom()
{
    this->_userInTheRoom.clear();
}

void Room::addUserToRoom(const LoggedUser& user)
{
    this->_userInTheRoom.push_back(user);
}
