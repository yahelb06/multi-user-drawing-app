#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* database) : m_database(database)
{
}

LoginStatus LoginManager::login(const std::string& name, const std::string& pass, SOCKET clientSocket)
{
    try
    {
        if (!this->m_database->doesPasswordMatch(name, pass))
        {
            return LoginStatus::LOGIN_FAILED;
        }
        std::lock_guard<std::mutex> lock(this->_loggedUser_mutex);
        if (this->isUserLoggedIn(name))
        {
            return LoginStatus::USER_ALREADY_LOGGED_IN;
        }
        LoggedUser newUser(name, clientSocket);
        this->m_loggedUsers.push_back(newUser);
        return LoginStatus::LOGIN_SUCCESS;
    }
    catch (...)
    {
        return LoginStatus::LOGIN_FAILED;
    }
}

SignUpStatus LoginManager::signup(const std::string& name, const std::string& pass, const std::string& mail, SOCKET clientSocket)
{
    try
    {
        if (this->m_database->addNewUser(name, pass, mail))
        {
            LoggedUser newUser(name, clientSocket);
            std::lock_guard<std::mutex> lock(this->_loggedUser_mutex);
            this->m_loggedUsers.push_back(newUser);
            return SignUpStatus::SIGNUP_SUCCESS;
        }
        //no user added
        return SignUpStatus::USER_ALREADY_EXISTS;
    }
    catch (...)
    {
        return SignUpStatus::SIGNUP_FAILED;
    }
}

UserLogOutStatus LoginManager::logout(const std::string& name, SOCKET clientSocket)
{
    std::lock_guard<std::mutex> lock(this->_loggedUser_mutex);
    return this->privateLogout(name, clientSocket);

}

RemoveStatus LoginManager::Remove(const std::string& name, SOCKET clientSocket)
{
    try
    {
        std::lock_guard<std::mutex> lock(this->_loggedUser_mutex);
        //user deleted
        if (this->m_database->deleteUser(name))
        {
            this->privateLogout(name, clientSocket);
            return RemoveStatus::REMOVE_SUCCESS;
        }
        //no user deleted
        return RemoveStatus::USER_NOT_FOUND;
    }
    catch (...)
    {
        return RemoveStatus::REMOVE_FAILED;
    }
}

SOCKET LoginManager::getSocketByUsername(const std::string& name) const
{
    std::lock_guard<std::mutex> lock(this->_loggedUser_mutex);

    for (const auto& user : this->m_loggedUsers)
    {
        if (user.getUserName() == name)
        {
            return user.getSocket();
        }
    }

    return INVALID_SOCKET;
}

std::string LoginManager::getUserBySocket(SOCKET clientSocket) const
{
    for (const auto& user : m_loggedUsers)
    {
        if (user.getSocket() == clientSocket)
        {
            return user.getUserName();
        }
    }
    return "";
}

bool LoginManager::isUserLoggedIn(const std::string& name)
{
    auto it = std::find_if(m_loggedUsers.begin(), m_loggedUsers.end(), [&name](const LoggedUser& user)
        {
            return user.getUserName() == name;
        });
    //found a user
    if (it != this->m_loggedUsers.end())
    {
        return true;
    }
    return false;
}

UserLogOutStatus LoginManager::privateLogout(const std::string& name, SOCKET clientSocket)
{
    LoggedUser userToRemove(name, clientSocket);
    auto it = std::remove(m_loggedUsers.begin(), m_loggedUsers.end(), userToRemove);
    //found user
    if (it != m_loggedUsers.end())
    {
        this->m_loggedUsers.erase(it, this->m_loggedUsers.end());
        return UserLogOutStatus::LOG_OUT_SUCCESS;
    }

    return UserLogOutStatus::LOG_OUT_FAILED;
}
