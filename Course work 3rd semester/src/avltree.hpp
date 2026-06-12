#ifndef MENU_TREE_H
#define MENU_TREE_H

#include <string>
#include <iostream>

struct Node{
    std::string name;
    int cost;
    int height;
    Node* left;
    Node* right;

    Node(std::string name, int cost) : 
        name(name), cost(cost), height(1), left(nullptr), right(nullptr) {}
};

class AvlTree{
    Node* rootNode;
    int resultCost;

    Node* balance(Node* root);
    int getBF(Node* root);
    void update(Node* root);
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* minimum(Node* root);
    Node* deleteNode(Node* root, std::string key);
    Node* insertNode(Node* root, std::string name, int cost);
    Node* search(Node* root, std::string name);
    void inOrderTraversal(Node* node);
    void destroyTree(Node* node);
    Node* makeDoubleLinkedList();

public:
    AvlTree();
    void insert(std::string name, int cost);
    void remove(std::string name);
    void show(std::string item);
    int searchByName(std::string name);
    void printLinkedList();
    ~AvlTree();
};

#endif