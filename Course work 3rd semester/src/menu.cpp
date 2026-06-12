#include "menu.hpp"

Menu::Menu() : user(User::CUSTOMER), menu(AvlTree()), customer(AvlTree()) {}

void Menu::help(){
    switch (user){
        case ADMIN:
            std::cout << "\nADMIN COMMAND SET\n";
            std::cout << "del <name> -- delete dish from the menu.\n";
            std::cout << "add <name> <price> -- add dish to the menu.\n";
            std::cout << "add <existing_name> <new_price> -- if you want to change th price of the existing dish.\n";
            break;

        case CUSTOMER:
            std::cout << "\nCUSTOMER COMMAND SET\n";
            std::cout << "select <name> -- add dish to cart.\n";
            std::cout << "del <name> -- delete dish from the cart.\n";
            std::cout << "cart -- show cart contents and total cost.\n";
            break;
            
        default: break;
    }
    std::cout << "menu -- show all available menu dishes.\n";
    std::cout << "search <name> -- search price by dishname in the menu.\n";
    std::cout << "help -- show command set.\n";
    std::cout << "exit -- close program.\n";
    std::cout << "admin -- if you are an admin, enter this command.\n\n";
}

int Menu::stringToInt(std::string str){
    int res = 0;
    for (char symb : str){
        if (symb > '9' || symb < '0')
            return -1;
        res = res*10 + (symb - '0');
    }
    return res;
}

void Menu::checkRights(){
    if (user != ADMIN){
        std::string code;
        std::cout << "Password: ";
        if (std::getline(std::cin, code)){
            if (stringToInt(code) == KEY){
                std::cout << "Success!\nIf you want to return to the customer mode, enter 'admin' again.\n";
                user = ADMIN;  
            } else std::cout << "Wrong password, try again.\n";
        }
    } else {
        std::cout << "Success! Change user to customer.\n";
        user = CUSTOMER; 
    }
}

void Menu::start(){
    std::string inputData;
    help();

    while (std::getline(std::cin, inputData)){
        if (inputData.empty()) continue;

        int firstSpace = (int)inputData.find(' ');

        if (firstSpace == (int)std::string::npos){
            if (inputData == "menu") menu.show(inputData);
            else if (inputData == "exit") break;
            else if (inputData == "help") help();
            else if (inputData == "admin") checkRights();
            else if (inputData == "cart" && user == CUSTOMER) customer.show(inputData);
            else if (inputData == "linkedList") menu.printLinkedList();
            else std::cout << "Unknown command. Print 'help' to get more information.\n";

        } else {
            std::string command = inputData.substr(0, firstSpace);
            std::string args = inputData.substr(firstSpace + 1);

            if (command == "search"){
                if (!args.empty()){
                    int cost = menu.searchByName(args);
                    if (cost != -1) std::cout << "> " << args << " costs " << cost << ".\n";
                } else std::cout << "Search error: enter the dishname.\n";
            
            } else if (command == "del"){
                if (!args.empty()){
                    if (user == ADMIN) menu.remove(args);
                    else if (user == CUSTOMER) customer.remove(args);
                    else std::cout << "User not defined.\n";

                } else std::cout << "Delete error: enter the dishname.\n";

            } else if (command == "add" && user == ADMIN){
                int lastSpace = args.find_last_of(' ');

                if (lastSpace == (int)std::string::npos)
                    std::cout << "Add error: enter the dishname and price.\n";
                else {
                    std::string dishName = args.substr(0, lastSpace);
                    int cost = stringToInt(args.substr(lastSpace + 1));

                    if (dishName.empty()) 
                        std::cout << "Add error: dishname can`t be skipped.\n";
                    else if (cost == -1)
                        std::cout << "Add error: invalid cost.\n";
                    else menu.insert(dishName, cost);
                }

            } else if (command == "select" && user == CUSTOMER){
                if (!args.empty()){
                    int cost = menu.searchByName(args);
                    if (cost != -1) customer.insert(args, cost);
                } else std::cout << "Select error: enter the dishname.\n";

            } else  std::cout << "Unknown command. Print 'help' to get more information.\n";
        }
    }
}