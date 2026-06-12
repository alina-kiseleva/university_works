#ifndef MENU_H
#define MENU_H

#include "avltree.hpp"

enum User{
    ADMIN,
    CUSTOMER
};

class Menu{
    User user;
    AvlTree menu;
    AvlTree customer;
    int KEY = 1503;

    void help();
    void checkRights();
    int stringToInt(std::string str);

public:
    Menu();
    void start();
};

#endif