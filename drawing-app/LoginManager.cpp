#include "LoginManager.h"

LoginManager::LoginManager(IDatabase* database) : m_database(database)
{
}

LoginStatus LoginManager::login(const std::string& name, const std::string& pass)
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
        LoggedUser newUser(name);
        this->m_loggedUsers.push_back(newUser);
        return LoginStatus::LOGIN_SUCCESS;
    }
    catch (...)
    {
        return LoginStatus::LOGIN_FAILED;
    }
}

SignUpStatus LoginManager::signup(const std::string& name, const std::string& pass, const std::string& mail)
{
    try
    {
        if (this->m_database->addNewUser(name, pass, mail))
        {
            LoggedUser newUser(name);
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

void LoginManager::logout(const std::string& name)
{
    std::lock_guard<std::mutex> lock(this->_loggedUser_mutex);
    this->privateLogout(name);
}

RemoveStatus LoginManager::Remove(const std::string& name)
{
    try
    {
        std::lock_guard<std::mutex> lock(this->_loggedUser_mutex);
        //user deleted
        if (this->m_database->deleteUser(name))
        {
            this->privateLogout(name);
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

bool LoginManager::isUserLoggedIn(const std::string& name) const
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

void LoginManager::privateLogout(const std::string& name)
{
    LoggedUser userToRemove(name);
    auto it = std::remove(m_loggedUsers.begin(), m_loggedUsers.end(), userToRemove);
    this->m_loggedUsers.erase(it, this->m_loggedUsers.end());
}
