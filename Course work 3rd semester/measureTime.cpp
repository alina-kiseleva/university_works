#include "avltree.hpp"
#include <chrono>
#include <fstream>
#include <vector>
#include <string>

int main() {
    std::ofstream out("results.txt");
    std::vector<int> sizes = {10, 50, 100, 250, 500, 750, 1000, 
                              2500, 5000, 7500, 10000, 12500, 
                              15000, 17500, 20000};
    
    out << "n insert_time delete_time search_time traversal_time\n";
    
    for (int n : sizes) {
        AvlTree tree;
        
        // Вставка
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i) {
            tree.insert("key" + std::to_string(i), i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        long insert_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / n;
        
        // Поиск
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i) {
            tree.searchByName("key" + std::to_string(i));
        }
        end = std::chrono::high_resolution_clock::now();
        long search_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / n;

        start = std::chrono::high_resolution_clock::now();
        tree.show("menu");
        end = std::chrono::high_resolution_clock::now();
        long traversal_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        
        // Удаление
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; ++i) {
            tree.remove("key" + std::to_string(i));
        }
        end = std::chrono::high_resolution_clock::now();
        long delete_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / n;

        out << n << " " << insert_time << " " << delete_time << " " 
            << search_time << " " << traversal_time << "\n";
    }
    
    out.close();
    return 0;
}