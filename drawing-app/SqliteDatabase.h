#pragma once
#include "IDatabase.h"

#define NOT_VALID_PAINT_NAME -1

class SqliteDatabase : public IDatabase
{
public:

	SqliteDatabase();
	~SqliteDatabase() override;

	bool open() override;
	void close() override;
	//bool doesUserExist(const std::string& name) const override;
	bool doesPasswordMatch(const std::string& name, const std::string& pass) const override;
	bool addNewUser(const std::string& name, const std::string& pass, const std::string& mail) override;
	bool deleteUser(const std::string& name) override;
	int getPaintId(const std::string& name, const std::string& paintName) const override;
	std::vector<std::string> GetUserPaintsName(const std::string name) const override;
	Paint GetPaint(const int& paintId, const std::string& paintName) const override;
	bool savePaint(const std::string& manager, const std::string paintName, const std::vector<Line>& linesToSave) const override;
	void addPaint(const std::string& manager, const std::string paintName, const std::vector<Line>& lines) const override;
	void insertLines(const int paintId, const std::vector<Line>& lines) const override;

private:

	mutable std::mutex _dbMutex;
	sqlite3* _db;
};