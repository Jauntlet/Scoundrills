#include <sqlite3.h>
#include <iostream>

#include "Database.h"

Database::Database() {
    sqlite3* database;
	
	sqlite3_open("bruh.db", &database);

	sqlite3_exec(database,	"CREATE TABLE IF NOT EXISTS MyTable (ID INT, NAME TEXT);", nullptr, nullptr, nullptr);

	sqlite3_exec(database, "INSERT INTO MyTable (ID, NAME) VALUES (1, 'John')", nullptr, nullptr, nullptr);

	std::cout << sqlite3_exec(database, "SELECT * FROM MyTable;", nullptr, nullptr, nullptr) << std::endl;

	sqlite3_close(database);
}