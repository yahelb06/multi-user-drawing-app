#pragma once
#include "Pch.h"

class LoggedUser
{
public:

	LoggedUser(const std::string& name);
	std::string getUserName() const;
	bool operator==(const LoggedUser& otherUser) const;
private:

	std::string m_userName;
};