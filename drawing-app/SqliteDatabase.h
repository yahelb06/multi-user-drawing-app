#pragma once
#include "IDatabase.h"

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

private:

	mutable std::mutex _dbMutex;
	sqlite3* _db;
};