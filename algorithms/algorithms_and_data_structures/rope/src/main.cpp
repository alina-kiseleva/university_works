#include <iostream>
#include <vector>

const int MAX_LEAF = 8;

struct Node{
    std::string str;
    int weight;
    int height;
    Node* left;
    Node* right;

    Node(std::string s = ""):str(s), weight(0), height(1), left(nullptr), right(nullptr){}
};

int getHeight(Node* n){ 
    return n ? n->height : 0; 
}

int getLength(Node* n){
    if (!n) return 0;
    if (!n->left && !n->right) return (int)n->str.size();
    return n->weight + getLength(n->right);
}

void update(Node* n){
    if (!n) return;
    n->weight = n->left ? getLength(n->left) : 0;
    int lh = getHeight(n->left);
    int rh = getHeight(n->right);
    n->height = (lh > rh ? lh : rh) + 1;
}

int getBalance(Node* n){
    return n ? getHeight(n->left) - getHeight(n->right) : 0;
}

Node* rotateRight(Node* y){
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    update(y);
    update(x);
    return x;
}

Node* rotateLeft(Node* x){
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    update(x);
    update(y);
    return y;
}

Node* balance(Node* node){
    if (!node) return node;
    update(node);
    int b = getBalance(node);

    if (b > 1) {
        if (getBalance(node->left) < 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (b < -1) {
        if (getBalance(node->right) > 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

Node* concat(Node* left, Node* right){
    if (!left) return right;
    if (!right) return left;
    Node* root = new Node();
    root->left = left;
    root->right = right;
    update(root);
    return balance(root);
}

std::pair<Node*, Node*> split(Node* node, int idx){
    if (!node) return {nullptr, nullptr};
    update(node);

    if (!node->left && !node->right){
        int sz = (int)node->str.size();
        if (idx >= sz)
            return {node, nullptr};
        if (idx <= 0)
            return {nullptr, node};
        std::string l = node->str.substr(0, idx);
        std::string r = node->str.substr(idx);
        return {new Node(l), new Node(r)};
    }

    if (idx < node->weight){
        auto p = split(node->left, idx);
        Node* newRight = concat(p.second, node->right);
        return {p.first, newRight};
    } else {
        auto p = split(node->right, idx - node->weight);
        Node* newLeft = concat(node->left, p.first);
        return {newLeft, p.second};
    }
}

Node* buildRopeFromRange(const std::string& s, int l, int r){
    int len = r - l;
    if (len <= MAX_LEAF) {
        return new Node(s.substr(l, len));
    }
    int mid = l + len / 2;
    Node* left = buildRopeFromRange(s, l, mid);
    Node* right = buildRopeFromRange(s, mid, r);
    return concat(left, right);
}

Node* buildRope(const std::string& s){
    if (s.empty()) return nullptr;
    return buildRopeFromRange(s, 0, (int)s.size());
}

Node* insert(Node* root, int idx, const std::string& s){
    auto p = split(root, idx);
    Node* mid = buildRope(s);
    return concat(concat(p.first, mid), p.second);
}

Node* deleteSubstr(Node* root, int start, int len){
    auto p1 = split(root, start);
    auto p2 = split(p1.second, len);
    return concat(p1.first, p2.second);
}

void collect(Node* n, std::string& res){
    if (!n) return;
    if (!n->left && !n->right) res += n->str;
    else {
        collect(n->left, res);
        collect(n->right, res);
    }
}

void printLeaves(Node* n, int &id){
    if (!n) return;
    if (!n->left && !n->right) {
        std::cout << "Leaf " << id++ << ": " << n->str << "\n";
    } else {
        printLeaves(n->left, id);
        printLeaves(n->right, id);
    }
}

void printNodes(Node* n, int& id){
    if (!n || (!n->left && !n->right)) return;
    std::cout << "Node " << id++ << ": " << n->weight << "\n";
    printNodes(n->left, id);
    printNodes(n->right, id);
}

void printRope(Node* root){
    int id = 1;
    printLeaves(root, id);
    id = 1;
    printNodes(root, id);
    std::string result;
    collect(root, result);
    std::cout << result << "\n";
}

int toInt(const std::string& s){
    int sign = 1;
    int i = 0;
    if (s.size() > 0 && s[0] == '-') { sign = -1; i = 1; }
    int val = 0;
    for (; i < (int)s.size(); ++i) {
        char c = s[i];
        if (c < '0' || c > '9') break;
        val = val * 10 + (c - '0');
    }
    return val * sign;
}

int main(){
    std::string text;
    std::getline(std::cin, text);

    std::string line2;
    std::getline(std::cin, line2);
    int pos = (int)line2.find(' ');
    int insertPos = 0;
    std::string insertStr = "";
    if (pos == (int)std::string::npos) {
        insertPos = toInt(line2);
        insertStr = "";
    } else {
        insertPos = toInt(line2.substr(0, pos));
        insertStr = line2.substr(pos + 1);
    }

    std::string line3;
    std::getline(std::cin, line3);
    int pos3 = (int)line3.find(' ');
    int delPos = 0, delLen = 0;
    if (pos3 == (int)std::string::npos) {
        delPos = toInt(line3);
        delLen = 0;
    } else {
        delPos = toInt(line3.substr(0, pos3));
        delLen = toInt(line3.substr(pos3 + 1));
    }

    Node* rope = buildRope(text);
    printRope(rope);

    rope = insert(rope, insertPos, insertStr);
    printRope(rope);

    rope = deleteSubstr(rope, delPos, delLen);
    printRope(rope);

    return 0;
}
