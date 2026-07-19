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
	std::lock_guard<std::mutex> lock(this->_dbMutex);
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
		std::vector<std::string> sqlStatements = { "CREATE TABLE USERS (NAME TEXT PRIMARY KEY NOT NULL , PASSWORD TEXT NOT NULL , MAIL TEXT NOT NULL UNIQUE);",

		"CREATE TABLE PAINTS (ID INTEGER PRIMARY KEY AUTOINCREMENT, USERNAME TEXT NOT NULL, PAINT_NAME TEXT NOT NULL);",

		"CREATE TABLE PAINT_LINES (LINE_ID INTEGER PRIMARY KEY AUTOINCREMENT, PAINT_ID INT NOT NULL, START_X INT NOT NULL, START_Y INT NOT NULL, "
			"END_X INT NOT NULL, END_Y INT NOT NULL, COLOR TEXT NOT NULL, FOREIGN KEY (PAINT_ID) REFERENCES PAINTS(ID));"};
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
	std::lock_guard<std::mutex> lock(this->_dbMutex);
	sqlite3_close(this->_db);
	this->_db = nullptr;
}

/*
bool SqliteDatabase::doesUserExist(const std::string& name) const
{
	std::lock_guard<std::mutex> lock(this->_dbMutex);
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
*/

bool SqliteDatabase::doesPasswordMatch(const std::string& name, const std::string& pass) const
{
	std::lock_guard<std::mutex> lock(this->_dbMutex);
	sqlite3_stmt* stmt;
	std::string sqlStatement = "SELECT * FROM USERS "
		"WHERE NAME = ? AND PASSWORD = ?;";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::runtime_error(sqlite3_errmsg(this->_db)));
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

bool SqliteDatabase::addNewUser(const std::string& name, const std::string& pass, const std::string& mail)
{
	std::lock_guard<std::mutex> lock(this->_dbMutex);
	sqlite3_stmt* stmt;
	std::string sqlStatement = "INSERT INTO USERS (NAME, PASSWORD, MAIL) "
		"VALUES(?, ?, ?);";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::runtime_error(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, mail.c_str(), -1, SQLITE_STATIC);
	res = sqlite3_step(stmt);
	if (res == SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		return true;
	}
	else if (res == SQLITE_CONSTRAINT)
	{
		sqlite3_finalize(stmt);
		return false;
	}
	throw std::runtime_error(sqlite3_errmsg(this->_db));
}

bool SqliteDatabase::deleteUser(const std::string& name)
{
	std::lock_guard<std::mutex> lock(this->_dbMutex);
	sqlite3_stmt* stmt;
	std::string sqlStatement = "DELETE FROM USERS "
		"WHERE NAME = ?;";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::runtime_error(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	return sqlite3_changes(this->_db);
}

int SqliteDatabase::getPaintId(const std::string& name, const std::string& paintName) const
{
	std::lock_guard<std::mutex> lock(this->_dbMutex);
	sqlite3_stmt* stmt;
	std::string sqlStatement = "SELECT ID FROM PAINTS "
		"WHERE USERNAME = ? AND PAINT_NAME = ?;";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, paintName.c_str(), -1, SQLITE_STATIC);
	
	int paintId;
	if (sqlite3_step(stmt) != SQLITE_ROW)
	{
		return -1;
	}
	paintId = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);
	return paintId;
}

std::vector<std::string> SqliteDatabase::GetUserPaintsName(const std::string name) const
{
	std::lock_guard<std::mutex> lock(this->_dbMutex);
	sqlite3_stmt* stmt;
	std::vector<std::string> PaintName;
	std::string sqlStatement = "SELECT PAINT_NAME FROM PAINTS "
		"WHERE USERNAME = ?;";

	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		PaintName.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
	}
	sqlite3_finalize(stmt);
	return PaintName;
}

Paint SqliteDatabase::GetPaint(const int& paintId, const std::string& paintName) const
{
	std::lock_guard<std::mutex> lock(this->_dbMutex);
	sqlite3_stmt* stmt;
	std::vector<Line> paintLines;
	std::string sqlStatement = "SELECT * FROM PAINT_LINES "
		"WHERE PAINT_ID = ?";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_int(stmt, 1, paintId);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		Coordinates start(static_cast<unsigned int>(sqlite3_column_int(stmt, 2)), static_cast<unsigned int>(sqlite3_column_int(stmt, 3)));
		Coordinates end(static_cast<unsigned int>(sqlite3_column_int(stmt, 4)), static_cast<unsigned int>(sqlite3_column_int(stmt, 5)));
		std::string color(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
		std::pair<Coordinates, Coordinates> lineCore(start, end);
		Line line(lineCore, color);
		paintLines.push_back(line);
	}
	sqlite3_finalize(stmt);
	return Paint(paintLines, paintName);
}

bool SqliteDatabase::savePaint(const std::string& manager, const std::string paintName, const std::vector<Line>& linesToSave) const
{
	int paintId = this->getPaintId(manager, paintName);
	if(NOT_VALID_PAINT_NAME == paintId)
	{
		try
		{
			this->addPaint(manager, paintName, linesToSave);
		}
		catch (const std::string& e)
		{
			return false;
		}
	}
	else
	{
		sqlite3_stmt* stmt;
		std::string sqlStatement = "DELETE FROM PAINT_LINES "
			"WHERE PAINT_ID = ?;";
		int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

		if (res != SQLITE_OK)
		{
			sqlite3_finalize(stmt);
			throw (std::string(sqlite3_errmsg(this->_db)));
		}
		sqlite3_bind_int(stmt, 1, paintId);
		if (sqlite3_step(stmt) == SQLITE_DONE)
		{
			try
			{
				this->insertLines(paintId, linesToSave);
				return true;
			}
			catch (const std::string& e)
			{
				return false;
			}
		}
		return false;
	}
}

void SqliteDatabase::addPaint(const std::string& manager, const std::string paintName, const std::vector<Line>& lines) const
{
	sqlite3_stmt* stmt;
	std::string sqlStatement = "INSERT INTO PAINTS (USERNAME, PAINT_NAME) "
		"VALUES (?, ?);";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);

	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	sqlite3_bind_text(stmt, 1, manager.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, paintName.c_str(), -1, SQLITE_STATIC);
	res = sqlite3_step(stmt);
	if (res != SQLITE_DONE)
	{
		std::string err = sqlite3_errmsg(this->_db);
		sqlite3_finalize(stmt);
		std::cout << err;
		throw (std::string("SQLite Insert Error: ") + err);
	}
	sqlite3_finalize(stmt);
	//took the id for the new paint
	sqlite3_int64 paintId = sqlite3_last_insert_rowid(this->_db);
	try
	{
		this->insertLines(paintId, lines);
	}
	catch (const std::string& e)
	{
		throw;
	}
}

void SqliteDatabase::insertLines(const int paintId, const std::vector<Line>& lines) const
{
	sqlite3_stmt* stmt;
	std::string sqlStatement = "INSERT INTO PAINT_LINES (PAINT_ID, START_X, START_Y, END_X, END_Y, COLOR) "
		"VALUES (?, ?, ?, ?, ?, ?);";
	int res = sqlite3_prepare_v2(this->_db, sqlStatement.c_str(), -1, &stmt, nullptr);
	if (res != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw (std::string(sqlite3_errmsg(this->_db)));
	}
	for (const auto& line : lines)
	{
		double startX = line.getLine().first.coordinates.first;
		double startY = line.getLine().first.coordinates.second;

		double endX = line.getLine().second.coordinates.first;
		double endY = line.getLine().second.coordinates.second;

		std::string color = line.getColor();

		sqlite3_bind_int64(stmt, 1, paintId);
		sqlite3_bind_double(stmt, 2, startX);
		sqlite3_bind_double(stmt, 3, startY);
		sqlite3_bind_double(stmt, 4, endX);
		sqlite3_bind_double(stmt, 5, endY);
		sqlite3_bind_text(stmt, 6, color.c_str(), -1, SQLITE_STATIC);

		res = sqlite3_step(stmt);

		if (res != SQLITE_DONE)
		{
			sqlite3_finalize(stmt);
			throw (std::string(sqlite3_errmsg(this->_db)));
		}
		sqlite3_reset(stmt);
		sqlite3_clear_bindings(stmt);
	}
	sqlite3_finalize(stmt);
}
