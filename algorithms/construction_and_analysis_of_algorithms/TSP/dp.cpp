#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

const int INF = 1e6;

void extractInputMatrix(int n, std::vector<std::vector<int>>& allWeights){  //Функция преобразования консольного ввода (получение введенной матрицы)
    std::string line;
    for (int i = 0; i < n; ++i){
        std::getline(std::cin, line);
        std::stringstream ss(line);
        std::vector<int> weightsForOne; //Веса для одной вершины
        int weight;

        while (ss >> weight){
            weightsForOne.push_back(static_cast<int>(weight));
        }

        allWeights.push_back(weightsForOne);
    }
}

void printDists(int n, std::vector<std::vector<int>>& distances, std::ofstream& file){  //Функция записи матрицы расстояний в файл
    int fieldWidth = 8;

    for (int i = -1; i < n; ++i){
        for (int j = 0; j < (1 << n); ++j){
            if (i == -1){
                 if (j == 0) file <<  ' ';
                file << std::setw(fieldWidth) << j;   //Первая строка вывода - маски
            } else {
                if (j == 0) file <<  i; // Первый столбец вывода - номера вершин
                file << std::setw(fieldWidth) << distances[i][j];
            }
            if (j == (1 << n) - 1) file << '\n';
        }
    }
    file << "_________________________________________________________________________________________________________________\n";
}

int findShortest(int vertex, int mask, int n, std::vector<std::vector<int>>& weights, std::vector<std::vector<int>>& dists, std::ofstream& file){    //Функция поиска кратчайшего гамильтонова цикла
    if (dists[vertex][mask] != INF){
        return dists[vertex][mask];
    }

    for (int j = 0; j < n; ++j){
        int maskBit = (mask >> j) & 1;  //j-й бит маски (если 1, нужно проверить путь через вершину j)
        if (weights[vertex][j] != 0 && maskBit == 1){
            dists[vertex][mask] = std::min(dists[vertex][mask], findShortest(j, mask ^ (1 << j), n, weights, dists, file) + weights[vertex][j]);

            file << "Текущая вершина i: " << vertex << '\n';
            file << "Вершина j из маски: " << j << '\n';
            file << "Маска: " << mask << '\n';
            file << "j-й бит маски: " << maskBit << '\n';
            file << "Минимальное расстояние из i через j в стартовую вершину: " << dists[vertex][mask] << '\n';
            file << "______________________________________________________________\n";
        }
    }

    return dists[vertex][mask];
}

std::vector<int> findWay(int start, int n, std::vector<std::vector<int>>& weights, std::vector<std::vector<int>>& dists, std::ofstream& file){  //Функция восстановления последовательности обхода по заполненному массиву расстояний
    int vertex = start;
    int mask = ((1 << n) - 1) ^ (1 << start);  //Начинаем с последней маски, не содержащей стартовую вершину
    std::vector<int> path = {vertex};

    while (mask != 0){
        for (int j = 0; j < n; ++j){
            int maskBit = (mask >> j) & 1;  //j-й бит маски
            if (weights[vertex][j] != 0 && maskBit == 1 && dists[vertex][mask] == dists[j][mask ^ (1 << j)] + weights[vertex][j]){

                file << "Текущая вершина i: " << vertex << '\n';
                file << "Mаска: " << mask << '\n';
                file << "j-й бит маски: " << maskBit << "\n\n";
                file << "Вес ребра из i в j (W): " << weights[vertex][j] << '\n';
                file << "Расстояние до старта из i через j (CUR): " << dists[vertex][mask] << '\n';
                file << "Расстояние до старта из j (PREV): " << dists[j][mask ^ (1 << j)] << "\n\n";
                file << "Проверка CUR = RPEV + W: " << dists[vertex][mask] << '=' << dists[j][mask ^ (1 << j)] << '+' << weights[vertex][j] << '\n';
                file << "Условие выполнено, вершина " << j << " добавлена в путь.\n";
                file << "_____________________________________________________\n";

                path.push_back(j);
                vertex = j;
                mask ^= (1 << j);   //Переход к маске, где вершина j посещена
                break;
            }
        }
    }
    path.push_back(start);  //Вставка стартовой вершины в ответ для завершенности цикла
    return path;
}

int main(){
    int n;
    int start; //Стартовая вершина
    std::string line;
    std::ofstream file("output.txt");

    std::cin >> n >> start;
    std::cin.ignore();

    std::vector<std::vector<int>> distances(n, std::vector<int>(1 << n, INF));  //Массив минимальных расстояний из i-й вершины в нулевую через вершину(-ы) j из маски
    std::vector<std::vector<int>> weights;  //Массив весов ребер
    
    extractInputMatrix(n, weights);

    for (int i = 0; i < n; ++i){
        distances[i][0] = (weights[i][start] == 0) ? INF : weights[i][start];   //Заполнение столбца массива расстояний для маски = 0 (база для рекурсии)
    }

    file << "Начальное состояние массива минимальных расстояний из i-й вершины в нулевую через вершину(-ы) j из маски\n";
    printDists(n, distances, file);

    file << "Поиск гамильтонова цикла\n";
    file << "_________________________________________________________________________________________________________________\n";

    int length = findShortest(start, ((1 << n) - 1) ^ (1 << start), n, weights, distances, file);    //Поиск минимального гамильтонова цикла среди всех масок, не содержащих 0 в начале пути

    file << "Итоговый массив расстояний\n";
    printDists(n, distances, file);

    if (length != INF){
        file << "Восстановление пути\n";
        file << "_________________________________________________________________________________________________________________\n";

        std::vector<int> cycle = findWay(start, n, weights, distances, file);   //Восстановление пути по массиву расстояний
        std::cout << length << '\n';

        file << "Итоговый путь: \n";

        for (int i = 0; i < cycle.size(); ++i){
            std::cout << cycle[i] << ' ';
            file << cycle[i] << ' ';
        }
        file << '\n';
        std::cout << '\n';
    
    } else {
        file << "Путь не существует!\n";
        std::cout << "no path\n";
    }

    file.close();
    return 0;
}
