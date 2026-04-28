#pragma once
#include "Pch.h"

class User
{
public:

	User() = default;
	User(const std::string& name, const std::string& password, const std::string& mail);

	std::string getName() const;
	void setName(const std::string& name);

	bool operator==(const User& other) const;
	bool operator==(int id) const;

private:

	std::string name;
	std::string password;
	std::string mail;
};