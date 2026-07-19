#pragma once
#include "sqlite3.h"
#include "Pch.h"
#include "Paint.h"

class IDatabase
{
public:
	virtual ~IDatabase() = default;
	virtual bool open() = 0;
	virtual void close() = 0;
	//virtual bool doesUserExist(const std::string& name) const = 0;
	virtual bool doesPasswordMatch(const std::string& name, const std::string& pass) const = 0;
	virtual bool addNewUser(const std::string& name, const std::string& pass, const std::string& mail) = 0;
	virtual bool deleteUser(const std::string& name) = 0;
	virtual int getPaintId(const std::string& name, const std::string& paintName) const = 0;
	virtual std::vector<std::string> GetUserPaintsName(const std::string name) const = 0;
	virtual Paint GetPaint(const int& paintId, const std::string& paintName) const = 0;
	virtual bool savePaint(const std::string& manager, const std::string paintName, const std::vector<Line>& linesToSave) const = 0;
	virtual void addPaint(const std::string& manager, const std::string paintName, const std::vector<Line>& lines) const = 0;
	virtual void insertLines(const int paintId, const std::vector<Line>& lines) const = 0;

};