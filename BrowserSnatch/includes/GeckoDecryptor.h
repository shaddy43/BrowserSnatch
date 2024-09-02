#ifndef GECKODECRYPTOR_H
#define GECKODECRYPTOR_H

#include "includes\Imports.h"

enum class SECItemType {
    siBuffer = 0,
    siClearDataBuffer = 1,
    siCipherDataBuffer,
    siDERCertBuffer,
    siEncodedCertBuffer,
    siDERNameBuffer,
    siEncodedNameBuffer,
    siAsciiNameString,
    siAsciiString,
    siDEROID,
    siUnsignedInteger,
    siUTCTime,
    siGeneralizedTime
};

struct SECItem {
    SECItemType type;
    unsigned char* data;
    size_t len;
};

using Pk11SdrDecrypt = int(SECItem*, SECItem*, void*);
using NssInit = long(char* sDirectory);
using NssShutdown = long();

class GeckoDecryptor {
private:

public:
    // Constructor
    GeckoDecryptor();

    // Destructor (if needed)
    ~GeckoDecryptor();

    // Member function to initialize ChromiumDecryptor
    bool GeckoDecryptorInit(std::string& target_mozilla_browser);
    bool set_profile_dir(std::string profile_dir);
    bool decrypt_data(const std::string& encrypted_data, std::string& decrypted_data);

private:
    // Helper functions if needed
    NssInit* m_NssInit = nullptr;
    Pk11SdrDecrypt* m_ipNssPk11SdrDecrypt = nullptr;
    NssShutdown* m_NssShutdown = nullptr;
    HMODULE m_hNss3 = nullptr;
    HMODULE m_hMozGlue = nullptr;
};

#endif /* GECKODECRYPTOR_H */