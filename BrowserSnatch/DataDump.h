#pragma once

#ifndef DATADUMP_H
#define DATADUMP_H

#include "Imports.h"
#include "DataHolder.h"

bool dump_data(const std::string& db_path, DataHolder* data_array, int data_index);

#endif // DATADUMP_H