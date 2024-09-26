//
// Created by Dorant Velija on 11.9.24.
//

#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <string>

void clearConsole();

class User {
private:
    std::string name;
    bool loggedIn;
    bool admin;
public:
    User(std::string name = "default", bool loggedIn = false, bool admin = false);
    std::string getName() const;
    bool isloggedIn() const;
    bool isAdmin() const;
    void setLogin();

    void setLogin(bool b);
};

std::string hashPassword(const std::string& password);

void registerUser();
User login();
void viewUsers(User &user);
void updateName(User &user);
void updatePassword(User &user);

#endif // USER_MANAGEMENT_H