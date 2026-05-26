#pragma once
#include "Pch.h"

class LoggedUser
{
public:

	LoggedUser(const std::string& name, SOCKET clientSocket);
	std::string getUserName() const;
	SOCKET getSocket() const;
	bool operator==(const LoggedUser& otherUser) const;
private:

	std::string m_userName;
	SOCKET m_socket;
};