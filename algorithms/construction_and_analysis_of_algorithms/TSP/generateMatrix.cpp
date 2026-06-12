#include <iostream>
#include <fstream>
#include <random>
#include <vector>


void generateMatrix(int n, bool isSymmetric){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 50);

    std::ofstream file("randomMatrix.txt");
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            if (i == j){
                matrix[i][j] = 0;

            } else {
                int randomNumber = dist(gen);
                matrix[i][j] = randomNumber;

                if (isSymmetric) matrix[j][i] = randomNumber;
            }
        }
    }

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j){
            file << matrix[i][j] << ' ';
            if (j == n-1) file << '\n';
        }
    }

    file.close();
}


int main(){
    int n;
    bool isSymmetric;
    std::cin >> n >> isSymmetric;
    generateMatrix(n, isSymmetric);
}