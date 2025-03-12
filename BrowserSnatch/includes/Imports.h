#pragma once
#ifndef IMPORTS_H
#define IMPORTS_H

#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <windows.h>
#include <wincrypt.h>
#include <bcrypt.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tlhelp32.h>
#include <locale>
#include <codecvt>
#include <regex>
#include <Shlobj.h>
#include "sqlite3.h"
#include <vector>
#include <cstdlib>
#include <map>
#include <ctime>
#include <filesystem>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include <taskschd.h>
#include <comdef.h>
#include <sstream>
#include <wrl/client.h>
#include <iomanip>
#include <winternl.h>
#include <wtsapi32.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "Wtsapi32.lib")  // Link WTS API

const int IV_SIZE = 12;
const int TAG_SIZE = 16;

using json = nlohmann::json;

#endif /* IMPORTS_H */
