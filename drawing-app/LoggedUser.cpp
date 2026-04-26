#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& name)
{
    this->m_userName = name;
}

std::string LoggedUser::getUserName() const
{
    return this->m_userName;
}

bool LoggedUser::operator==(const LoggedUser& otherUser) const
{
    return this->m_userName == otherUser.getUserName();
}
