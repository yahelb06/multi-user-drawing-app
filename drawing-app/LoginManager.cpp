#include "LoginManager.h"

bool LoginManager::isUserLoggedIn(const std::string& name) const
{
    auto it = std::find_if(m_loggedUsers.begin(), m_loggedUsers.end(), [&name](const LoggedUser& user) 
        {
            return user.getUserName() == name;
        });
    if (it != this->m_loggedUsers.end())
    {
        return true;
    }
    return false;
}
