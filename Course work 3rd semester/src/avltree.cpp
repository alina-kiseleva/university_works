#include "avltree.hpp"

AvlTree::AvlTree():rootNode(nullptr), resultCost(0) {}

Node* AvlTree::balance(Node* root){
    if (!root)
        return root;
    update(root);
    int balanceFactor = getBF(root);

    if (balanceFactor < -1){
        if (getBF(root->left) > 0)
            root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    else if (balanceFactor > 1){
        if (getBF(root->right) < 0)
            root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

int AvlTree::getBF(Node* root){
    if (!root) return 0;
    int leftHeight = root->left ? root->left->height : 0;
    int rightHeight = root->right ? root->right->height : 0;
    return rightHeight - leftHeight;
}

void AvlTree::update(Node* root){
    if (!root) return;
    int lh = (root->left) ? root->left->height : 0;
    int rh = (root->right) ? root->right->height : 0;
    root->height = (lh > rh) ? lh + 1 : rh + 1;
}

Node* AvlTree::rotateLeft(Node* root){
    Node* rightChild = root->right;
    
    root->right = rightChild->left;
    rightChild->left = root;

    update(root);
    update(rightChild);
    return rightChild;
}

Node* AvlTree::rotateRight(Node* root){
    Node* leftChild = root->left;
    
    root->left = leftChild->right;
    leftChild->right = root;

    update(root);
    update(leftChild);
    return leftChild;
}

Node* AvlTree::insertNode(Node* root, std::string name, int cost){
    if (root == nullptr){
        root = new Node(name, cost);
        return root;
    }
    if (name.compare(root->name) < 0)
        root->left = insertNode(root->left, name, cost);
    else if (name.compare(root->name) > 0)
        root->right = insertNode(root->right, name, cost);
    else root->cost = cost;

    return balance(root);
}

Node* AvlTree::deleteNode(Node* root, std::string key){
    if (!root) return root;
    if (key.compare(root->name) < 0)
        root->left = deleteNode(root->left, key);
    else if (key.compare(root->name) > 0)
        root->right = deleteNode(root->right, key);
    else if (root->left != nullptr && root->right != nullptr){
        Node* minRight = minimum(root->right);
        root->name = minRight->name;
        root->cost = minRight->cost;
        root->right = deleteNode(root->right, root->name);
    } else {
        Node* tmp = root;
        if (root->left == nullptr)
            root = root->right;
        else root = root->left;
        delete tmp;
    }
    return balance(root);
}

Node* AvlTree::minimum(Node* root){
    if (root) {
    while (root->left){
        root = root->left;
        }
    }
    return root;
}

Node* AvlTree::search(Node* root, std::string name){
    if (!root) return nullptr;

    if (name.compare(root->name) < 0)
        return search(root->left, name);
    else if (name.compare(root->name) > 0) 
        return search(root->right, name);
    
    return root;
}

void AvlTree::destroyTree(Node* node){
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

void AvlTree::inOrderTraversal(Node* node){
    if (!node) return;
    inOrderTraversal(node->left);
    resultCost += node->cost;
    std::cout << node->name << ' ' << "--" << ' ' << node->cost << '\n';
    inOrderTraversal(node->right);
}

void AvlTree::insert(std::string name, int cost){
    rootNode = insertNode(rootNode, name, cost);
}

void AvlTree::remove(std::string name){
    if (!search(rootNode, name)) 
        std::cout << "Delete error: dish not exist.\n";
    else rootNode = deleteNode(rootNode, name);
}

void AvlTree::show(std::string item){
    if (item == "menu") std::cout << "\nCANTEEN MARIA MENU\n";
    else if (item == "cart") std::cout << "\nCART\n";

    inOrderTraversal(rootNode);
    
    if (item == "cart") std::cout << "Result cost: " << resultCost << '\n';
    std::cout << '\n';
    resultCost = 0;
}

int AvlTree::searchByName(std::string name){
    Node* node = search(rootNode, name);
    if (node == nullptr){
        std::cout << "Search error: dish is not on the menu.\n";
        return -1;
    } else return node->cost; 
}

Node* AvlTree::makeDoubleLinkedList(){
    if (!rootNode) return nullptr;

    Node* current = rootNode;
    Node* prev = nullptr;
    Node* head = nullptr;

    while (current){
        if (current->left == nullptr){
            if (!prev)
                head = current;
            else {
                prev->right = current;
                current->left = prev;
            }
            prev = current;
            current = current->right;
        } else {
            Node* prevInOrder = current->left;
            while(prevInOrder->right && prevInOrder->right != current)
                prevInOrder = prevInOrder->right;
                
            if (prevInOrder->right == nullptr){
                prevInOrder->right = current;
                current = current->left;
            
            } else {
                prevInOrder->right = nullptr;

                if (!prev){
                    head = current;
                } else {
                    prev->right = current;
                    current->left = prev;
                }
                prev = current;
                current = current->right;
            }
        }
    }
    return head;
}   

void AvlTree::printLinkedList(){
    Node* head = makeDoubleLinkedList();
    while (head){
        std::cout << head->name << " -- " << head->cost << '\n';
        head = head->right;
    }
}

AvlTree::~AvlTree(){
    destroyTree(rootNode);
    rootNode = nullptr;
}