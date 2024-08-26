#include "Imports.h"

class DataHolder {
private:
    std::string url;
    std::string username;
    std::string password;

public:
    // Constructors
    DataHolder() {}
    DataHolder(const std::string& url, const std::string& username, const std::string& password)
        : url(url), username(username), password(password) {}

    // Getter and setter methods (same as previous implementation)
    void setUrl(std::string newUrl) {
        url = newUrl;
    }

    std::string getUrl() {
        return url;
    }

    void setUsername(std::string newUsername) {
        username = newUsername;
    }

    std::string getUsername() {
        return username;
    }

    void setPassword(std::string newPassword) {
        password = newPassword;
    }

    std::string getPassword() {
        return password;
    }

    void print_data()
    {
        std::cout << "URL: " << url << std::endl;
        std::cout << "USERNAME: " << username << std::endl;
        std::cout << "PASSWORD: " << password << "\n" << std::endl;
    }
};