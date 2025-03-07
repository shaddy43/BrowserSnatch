#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "includes/Imports.h"

class Visualizer {

public:
    Visualizer();
    Visualizer(const std::string& converted_username, const std::string& stealer_db);
    ~Visualizer();

    // Function to start the visualization
    void visualization_main();
    BOOL default_settings(std::string converted_username, std::string stealer_db);
    BOOL greed_mode(std::string converted_username, std::string stealer_db);
    void displayHelp();
    void handler(int option, std::string service_parameter);

private:
    void clearConsole();
    void printDivider(char ch, int length);
    void printMenu();
    void handleUserChoice(int choice);
    
    std::string converted_username;
    std::string stealer_db;
};

#endif // VISUALIZER_H
#pragma once
