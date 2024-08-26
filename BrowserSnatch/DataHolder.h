#pragma once

#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <string>
#include <iostream>

class DataHolder {
private:
    std::string url;
    std::string username;
    std::string password;
    //std::string cookies;

public:
    // Constructors
    DataHolder() {}
    DataHolder(const std::string& url, const std::string& username, const std::string& password)
        : url(url), username(username), password(password) {}

    // Getter and setter methods
    void setUrl(const std::string& newUrl) {
        url = newUrl;
    }

    std::string getUrl() const {
        return url;
    }

    void setUsername(const std::string& newUsername) {
        username = newUsername;
    }

    std::string getUsername() const {
        return username;
    }

    void setPassword(const std::string& newPassword) {
        password = newPassword;
    }

    std::string getPassword() const {
        return password;
    }

    void printData() const {
        std::cout << "URL: " << url << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Password: " << password << "\n" << std::endl;
    }
};

#endif // DATAHOLDER_H

