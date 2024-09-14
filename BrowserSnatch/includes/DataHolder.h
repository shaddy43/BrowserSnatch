#pragma once

#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <iostream>
#include <string>

class DataHolder {
public:
    // Nested class for Passwords
    class Passwords {
    private:
        std::string url;
        std::string username;
        std::string password;
        std::string host;

    public:
        // Getters
        std::string getUrl() const { return url; }
        std::string getUsername() const { return username; }
        std::string getPassword() const { return password; }
        std::string getHost() const { return host; }

        // Setters
        void setUrl(const std::string& u) { url = u; }
        void setUsername(const std::string& user) { username = user; }
        void setPassword(const std::string& pass) { password = pass; }
        void setHost(const std::string& h) { host = h; }
    };

    // Nested class for Cookies
    class Cookies {
    private:
        std::string url;
        std::string host;
        std::string cookies;
        std::string cookieName;
        std::string cookiesExpiry;

    public:
        // Getters
        std::string getUrl() const { return url; }
        std::string getHost() const { return host; }
        std::string getCookies() const { return cookies; }
        std::string getCookieName() const { return cookieName; }
        std::string getCookiesExpiry() const { return cookiesExpiry; }

        // Setters
        void setUrl(const std::string& u) { url = u; }
        void setHost(const std::string& h) { host = h; }
        void setCookies(const std::string& c) { cookies = c; }
        void setCookieName(const std::string& name) { cookieName = name; }
        void setCookiesExpiry(const std::string& expiry) { cookiesExpiry = expiry; }
    };

    // Nested class for Bookmark
    class Bookmark {
    private:
        std::string date_added;
        std::string name;
        std::string type;
        std::string url;
        std::string host;

    public:
        // Getters
        std::string getDateAdded() const { return date_added; }
        std::string getName() const { return name; }
        std::string getType() const { return type; }
        std::string getUrl() const { return url; }
        std::string getHost() const { return host; }

        // Setters
        void setDateAdded(const std::string& date) { date_added = date; }
        void setName(const std::string& n) { name = n; }
        void setType(const std::string& t) { type = t; }
        void setUrl(const std::string& u) { url = u; }
        void setHost(const std::string& h) { host = h; }
    };

    // Nested class for History
    class History {
    private:
        std::string url;
        std::string title;
        std::string visit_count;
        std::string last_visit_time;
        std::string host;

    public:
        // Getters
        std::string getVisitCount() const { return visit_count; }
        std::string getTitle() const { return title; }
        std::string getLastVisitTime() const { return last_visit_time; }
        std::string getUrl() const { return url; }
        std::string getHost() const { return host; }

        // Setters
        void setVisitCount(const std::string& c) { visit_count = c; }
        void setTitle(const std::string& t) { title = t; }
        void setLastVisitTime(const std::string& l) { last_visit_time = l; }
        void setUrl(const std::string& u) { url = u; }
        void setHost(const std::string& h) { host = h; }
    };

private:
    Passwords password_manager;
    Cookies cookies_manager;
    Bookmark bookmarks_manager;
    History history_manager;

public:
    // Accessors for nested classes
    Passwords& get_password_manager() { return password_manager; }
    Cookies& get_cookies_manager() { return cookies_manager; }
    Bookmark& get_bookmarks_manager() { return bookmarks_manager; }
    History& get_history_manager() { return history_manager; }
};

#endif // DATAHOLDER_H

