#include "SqliteDatabase.h"

SqliteDatabase::SqliteDatabase()
{
	open();
}

SqliteDatabase::~SqliteDatabase()
{
	close();
}

bool SqliteDatabase::open()
{
	const int FILE_DONT_EXIST = -1;
	int fileExist = _access(DB_FILE_NAME, 0);
	char* errMessage = nullptr;

	int res = sqlite3_open(DB_FILE_NAME, &this->_db);
	if (res != SQLITE_OK)
	{
		this->_db = nullptr;
		sqlite3_free(errMessage);
		throw ("cannot open the db\n");
	}
	if (fileExist == FILE_DONT_EXIST)
	{
		std::vector<std::string> sqlStatements = { "CREATE TABLE USERS (NAME TEXT PRIMARY KEY NOT NULL , PASSWORD TEXT NOT NULL , MAIL TEXT NOT NULL);"};
		for (const auto& sqlStatement : sqlStatements)
		{
			errMessage = nullptr;
			int res = sqlite3_exec(this->_db, sqlStatement.c_str(), nullptr, nullptr, &errMessage);
			if (res != SQLITE_OK)
			{
				this->_db = nullptr;
				std::string message = errMessage;
				sqlite3_free(errMessage);
				throw (message);
			}
		}
	}
	return true;
}

void SqliteDatabase::close()
{
	sqlite3_close(this->_db);
	this->_db = nullptr;
}

bool SqliteDatabase::doesUserExist(const std::string& name) const
{
	sqlite3_stmt* stmt;
	std::string sqlStatement = "SELECT * FROM USERS "
		"WHERE NAME = ?;";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

	//found a user
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		sqlite3_finalize(stmt);
		return true;
	}
	sqlite3_finalize(stmt);
	return false;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& name, const std::string& pass) const
{
	sqlite3_stmt* stmt;
	std::string sqlStatement = "SELECT * FROM USERS "
		"WHERE NAME = ? AND PASSWORD = ?;";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		sqlite3_finalize(stmt);
		return true;
	}
	sqlite3_finalize(stmt);
	return false;
}

void SqliteDatabase::addNewUser(const std::string& name, const std::string& pass, const std::string& mail)
{
	sqlite3_stmt* stmt;
	std::string sqlStatement = "INSERT INTO USERS (NAME, PASSWORD, MAIL) "
		"VALUES(?, ?, ?);";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, mail.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(stmt) == SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
	}
	else
	{
		sqlite3_finalize(stmt);
		throw (sqlite3_errmsg(this->_db));
	}
}

void SqliteDatabase::deleteUser(const std::string& name)
{
	sqlite3_stmt* stmt;
	std::string sqlStatement = "REMOVE FROM USERS "
		"WHERE NAME = ?;";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		sqlite3_finalize(stmt);
	}
	else
	{
		sqlite3_finalize(stmt);
		throw(sqlite3_errmsg(this->_db));
	}
}
