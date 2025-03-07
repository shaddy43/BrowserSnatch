#pragma once
#ifndef IMPORTS_H
#define IMPORTS_H

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
#include <string.h>
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

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")


const int IV_SIZE = 12;
const int TAG_SIZE = 16;

using json = nlohmann::json;

#endif /* IMPORTS_H */
