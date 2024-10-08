//
// Created by Dorant Velija on 11.9.24.
//
#include "user_management.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

void clearConsole() {
#ifdef _WIN32
    system("cls");  // Windows
#else
    system("clear");  // Unix-based systems
#endif
}

User::User(std::string name, bool loggedIn, bool admin)
        : name(std::move(name)), loggedIn(loggedIn), admin(admin) {}

std::string User::getName() const {
    return name;
}

bool User::isloggedIn() const {
    return loggedIn;
}
void User::setLogin(bool b){
    this->loggedIn = b;
}

bool User::isAdmin() const {
    return admin;
}

std::string hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

void registerUser() {
    clearConsole();
    std::string name, password;
    int perms;
    bool permissions = false;
    bool exists = false;

    std::cout << "Enter name: ";
    std::cin >> name;
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    std::cout << "Enter password: ";
    std::cin >> password;
    std::string hashedPassword = hashPassword(password);
    std::cout << "Admin permissions (1-yes 0-no): ";
    std::cin >> perms;
    permissions = (perms == 1);

    std::ifstream check("users.txt");
    if (check) {
        std::string fileName, filePassword;
        bool filePermissions;
        while (check >> fileName >> filePassword >> filePermissions) {
            if (fileName == name) {
                exists = true;
                std::cout << "Error. User already exists.\n";
                check.close();
                break;
            }
        }
        check.close();
    }

    if (!exists) {
        std::ofstream file("users.txt", std::ios::app);
        if (file) {
            file << name << " " << hashedPassword << " " << permissions << std::endl;
            std::cout << "Registered successfully.\n";
            file.close();
        }
    }
}

User login() {
    clearConsole();
    std::string name, password;

    std::cout << "Enter name: ";
    std::cin >> name;
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    std::cout << "Enter password: ";
    std::cin >> password;
    std::string hashedPassword = hashPassword(password);

    std::ifstream file("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt");
    if (file) {
        std::string fileName, filePassword;
        bool permissions;
        while (file >> fileName >> filePassword >> permissions) {
            if (fileName == name && filePassword == hashedPassword) {
                file.close();
                name[0] = std::toupper(name[0]);
                std::cout << "Successfully logged in. Welcome " << name << ".\n";
                return User(fileName, true, permissions);
            }
        }
        file.close();
    }
    std::cout << "Invalid name or password. Try again.\n";
    return User("error", false, false);
}

void viewUsers(User &user) {
    clearConsole();
    if (user.isAdmin()) {
        std::ifstream file("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt");
        if (file) {
            int k;
            bool permissions;
            std::string fileName, fileP;

            while (file >> fileName >> fileP >> permissions) {
                std::cout << "Name: " << fileName << "\nAdmin: " << (permissions ? "yes" : "no") << "\n\n";
            }
            file.close();
            std::cout<<"\nPress 0 to go back: ";
            std::cin>>k;
            while ( k != 0){
                std::cout<<"\nPress 0 to go back: ";
                std::cin>>k;
            }

        }
    } else {
        clearConsole();
        std::cout << "Error. No permissions to view all users\n";
    }
}

void updateName(User &user){
    clearConsole();
    if (user.isloggedIn()){
        bool exists = false, confirmed = false;
        std::string password, name = user.getName();

        std::cout << "Enter your password: ";
        std::cin >> password;
        std::string hashedPassword = hashPassword(password);

        std::ifstream file("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt");
        if (file) {
            std::string fileName, filePassword;
            bool permissions;
            while (file >> fileName >> filePassword >> permissions) {
                if (fileName == name && filePassword == hashedPassword) {
                    confirmed = true;
                    file.close();
                }
            }
        }

        if (confirmed){
            std::cout<<"\nEnter your new username: ";
            std::cin>>name;
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            std::ifstream check("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt");
            if (check) {
                std::string fileName, filePassword;
                bool filePermissions;
                while (check >> fileName >> filePassword >> filePermissions) {
                    if (fileName == name) {
                        exists = true;
                        std::cout << "Error. Choose a new username.\n";
                        break;
                    }
                }
                check.close();
            }

            if (!exists) {
                std::ifstream inFile("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt");
                std::ofstream outFile("/Users/dorantvelija/CLionProjects/login-register-cpp/users_temp.txt");
                if (!inFile || !outFile) {
                    std::cerr << "Error opening file.\n";
                    return;
                }

                std::string fileName, filePassword;
                bool filePermissions;
                while (inFile >> fileName >> filePassword >> filePermissions) {
                    if (fileName == user.getName()) {
                        outFile << name << " " << filePassword << " " << filePermissions << "\n";
                    } else {
                        outFile << fileName << " " << filePassword << " " << filePermissions << "\n";
                    }
                }
                inFile.close();
                outFile.close();

                if (std::remove("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt") != 0) {
                    std::cerr << "Error deleting old file.\n";
                } else if (std::rename("/Users/dorantvelija/CLionProjects/login-register-cpp/users_temp.txt", "/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt") != 0) {
                    std::cerr << "Error renaming temporary file.\n";
                } else {
                    std::cout << "Username updated successfully!\n";
                }
            }
            } else {
            std::cout << "Error. Incorrect password.\n";
            }
        }
}

void updatePassword(User &user){
    clearConsole();
    if (user.isloggedIn()){
        bool confirmed = false;
        std::string password, name = user.getName(), confirmPassword;

        std::cout << "Enter your current password: ";
        std::cin >> password;
        std::string hashedPassword = hashPassword(password);

        std::ifstream file("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt");
        if (file) {
            std::string fileName, filePassword;
            bool permissions;
            while (file >> fileName >> filePassword >> permissions) {
                if (fileName == name && filePassword == hashedPassword) {
                    confirmed = true;
                    file.close();
                }
            }
        }

        if (confirmed){
            std::cout << "Enter your new password: ";
            std::cin >> password;
            hashedPassword = hashPassword(password);
            std::cout << "Confirm your password: ";
            std::cin >> confirmPassword;

            if (password == confirmPassword){
                std::ifstream inFile("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt");
                std::ofstream outFile("/Users/dorantvelija/CLionProjects/login-register-cpp/users_temp.txt");
                if (!inFile || !outFile) {
                    std::cerr << "Error opening file.\n";
                    return;
                }

                std::string fileName, filePassword;
                bool filePermissions;
                while (inFile >> fileName >> filePassword >> filePermissions) {
                    if (fileName == user.getName()) {
                        outFile << fileName << " " << hashedPassword << " " << filePermissions << "\n";
                    } else {
                        outFile << fileName << " " << filePassword << " " << filePermissions << "\n";
                    }
                }
                inFile.close();
                outFile.close();
                clearConsole();
                if (std::remove("/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt") != 0) {
                    std::cerr << "Error deleting old file.\n";
                } else if (std::rename("/Users/dorantvelija/CLionProjects/login-register-cpp/users_temp.txt", "/Users/dorantvelija/CLionProjects/login-register-cpp/users.txt") != 0) {
                    std::cerr << "Error renaming temporary file.\n";
                } else {
                    std::cout << "Password updated successfully!\n";
                    user.setLogin(false);
                }
            }

            else{
                std::cout<<"Passwords don't match.";
            }
        }
    }
    else{
        clearConsole();
        std::cout<<"Error. Not logged in.";
    }
}