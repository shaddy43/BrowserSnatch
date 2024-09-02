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
    std::string host;
    std::string cookies;
    std::string cookie_name;
    std::string cookie_expiry;

public:
    // Constructors
    DataHolder() {}
    DataHolder(const std::string& url, const std::string& username, const std::string& password)
        : url(url), username(username), password(password), host(host), cookies(cookies), cookie_name(cookie_name), cookie_expiry(cookie_expiry) {}

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

    void setHost(const std::string& newHost) {
        host = newHost;
    }

    std::string getHost() const {
        return host;
    }

    void setCookies(const std::string& newCookies) {
        cookies = newCookies;
    }

    std::string getCookies() const {
        return cookies;
    }

    void setCookieExpiry(const std::string& newCookieExpiry) {
        cookie_expiry = newCookieExpiry;
    }

    std::string getCookieExpiry() const {
        return cookie_expiry;
    }

    void setCookieName(const std::string& newCookieName) {
        cookie_name = newCookieName;
    }

    std::string getCookieName() const {
        return cookie_name;
    }

    void printData() const {
        std::cout << "URL: " << url << std::endl;
        std::cout << "Username: " << username << std::endl;
        std::cout << "Password: " << password << std::endl;
        std::cout << "Cookies: " << cookies << std::endl;
        std::cout << "Cookie Name: " << cookie_name << std::endl;
        std::cout << "Cookie Expiry: " << cookie_expiry << "\n" << std::endl;
    }
};

#endif // DATAHOLDER_H

