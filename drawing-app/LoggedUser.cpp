#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& name, SOCKET clientSocket) : m_userName(name), m_socket(clientSocket)
{
}

std::string LoggedUser::getUserName() const
{
    return this->m_userName;
}

SOCKET LoggedUser::getSocket() const
{
    return this->m_socket;
}

bool LoggedUser::operator==(const LoggedUser& otherUser) const
{
    return this->m_userName == otherUser.getUserName();
}
