#include "main.cpp"
#include <chrono>
#include <fstream>
#include <random>

std::string randomString(int n) {
    static const std::string chars = "abcdefghijklmnopqrstuvwxyz";
    std::string s;
    s.reserve(n);
    for (int i = 0; i < n; ++i)
        s += chars[rand() % chars.size()];
    return s;
}

char indexAt(Node* node, int idx) {
    if (!node) return '\0';

    if (!node->left && !node->right) {
        if (idx < 0 || idx >= (int)node->str.size()) return '\0';
        return node->str[idx];
    }

    if (idx < node->weight) {
        return indexAt(node->left, idx);
    } 
    else {
        return indexAt(node->right, idx - node->weight);
    }
}


int main() {
    std::ofstream out("results.txt");
    std::vector<int> sizes = {100, 1000, 10000, 100000, 1000000};

    for (int n : sizes) {
        std::string s = randomString(n);
        Node* rope = buildRope(s);
        int mid = n / 2;
        std::string ins = "abcde";

        auto start = std::chrono::high_resolution_clock::now();
        Node* t1 = insert(rope, mid, ins);
        auto end = std::chrono::high_resolution_clock::now();
        double insert_time = std::chrono::duration<double, std::micro>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        Node* t2 = deleteSubstr(t1, mid, 5);
        end = std::chrono::high_resolution_clock::now();
        double delete_time = std::chrono::duration<double, std::micro>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        char c = indexAt(t1, mid);
        end = std::chrono::high_resolution_clock::now();
        double index_time = std::chrono::duration<double, std::micro>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        Node* t3 = concat(t1, t2);
        end = std::chrono::high_resolution_clock::now();
        double concat_time = std::chrono::duration<double, std::micro>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        auto sp = split(t1, mid);
        end = std::chrono::high_resolution_clock::now();
        double split_time = std::chrono::duration<double, std::micro>(end - start).count();

        out << n << " "
            << insert_time << " "
            << delete_time << " "
            << index_time << " "
            << concat_time << " "
            << split_time << "\n";
    }

    out.close();
    return 0;
}
