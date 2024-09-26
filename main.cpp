#include <iostream>
#include "user_management.h"

int main() {
    clearConsole();
    bool k = true;
    int k1;
    User user;

    while (k) {
        std::cout << "Welcome.\n1. Login\n2. Register\n0. Exit\n";
        std::cin >> k1;
        if (k1 == 1) {
            user = login();
            if (user.isloggedIn()) {
                break;
            }
        }
        else if (k1 == 2) {
            registerUser();
        }
        else if (k1 == 0) {
            k = false;
        }
        else {
            std::cout << "Invalid option. Try again.\n";
        }
    }

    if (user.isloggedIn() == false ){
        return 0;
    }
    clearConsole();

    while (user.isloggedIn()){
        std::string name = user.getName();
        name[0] = toupper(name[0]);
        std::cout<<"Welcome "<< name <<"\n";
        std::cout<<"0.Exit\n1.View all users.\n2.Update Name\n3.Update password\n";
        std::cin>>k1;
        if (k1 == 0){
            return 0;
        }
        else if (k1 == 1){
            viewUsers(user);
            clearConsole();
        }
        else if (k1==2){
            updateName(user);
            user.setLogin(false);
        }
        else if (k1==3){
            updatePassword(user);
        }
    }

    return 0;
}