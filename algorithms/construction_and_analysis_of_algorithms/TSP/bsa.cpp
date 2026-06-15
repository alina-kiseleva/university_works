#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>

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

int sum(std::vector<std::pair<int, int>> arr){  //Сумма значений целочисленного вектора
    int s = 0;
    for (auto val : arr)
        s += val.first;
    return s;
}

double lowerBound(int start, int candidate, int n, std::vector<std::vector<int>> weights, std::vector<bool> visited, std::ofstream& file){   //Функция вычисления нижней оценки для добавляемого кандидата
    int minIn = INF;
    int minOut = INF;

    //Поиск для большого первого куска
    for (int i = 0; i < n; ++i){
        if (visited[i]) continue;

        int outgoing = weights[candidate][i];
        int incoming = weights[i][start];

        if (outgoing == 0 || incoming == 0) continue; //Пропуск нулевых ребер

        if (outgoing < minOut) minOut = outgoing;
        if (incoming < minIn) minIn = incoming;
    }

    file << "Вершина-кандидат на конец большого куска: " << candidate << '\n';
    file << "Вес минимального исходящего ребра из конца большого куска: " << minOut << '\n';
    file << "Вес минимального входящего ребра в начало большого куска: " << minIn << "\n\n";

    //Поиск для мелких одиночных кусков
    std::vector<std::pair<int, int>> minInForSingle; //Для всех одиночных кусков в порядке рассмотрения
    std::vector<std::pair<int, int>> minOutForSingle;

    //Кандидат - претендент на конец, его включили в большой кусок (временно)

    for (int j = 0; j < n; ++j){    //j - одиночные куски (не кандидат и не посещенные) 
        if (j == candidate || visited[j]) continue; 
        //Для конкретного одиночного куска 
        std::vector<std::pair<int, int>> in;    //массив пар <вес вершина_входящего_ребра>
        std::vector<std::pair<int, int>> out;   //массив пар <вес вершина_исходящего_ребра>

        for (int k = 0; k < n; ++k){    //k - допустимые ребра для них (через старт большого куска, конец (кандидат), непосещенные)
            if (k == start || k == candidate || !visited[k]){
                if (weights[j][k] == 0 || weights[k][j] == 0) continue; //пропускаем нулевые ребра

                in.push_back({weights[k][j], k});
                out.push_back({weights[j][k], k});
            }
        }

        file << "Одиночный кусок " << j << '\n';

        //Ищем минимальные неравные исходящие и входящие
        std::sort(in.begin(), in.end());
        std::sort(out.begin(), out.end());

        if (in[0].second == out[0].second){   //Разрешение конфликта совпадения минимального входящего и минимального исходящего
            if (out.size() > 1 && in.size() > 1){
                if (in[0].first + out[1].first < in[1].first + out[0].first){
                    minInForSingle.push_back(in[0]);
                    minOutForSingle.push_back(out[1]);
                } else {
                    minInForSingle.push_back(in[1]);
                    minOutForSingle.push_back(out[0]);
                }
            } else {
                minInForSingle.push_back({INF, -1});
                minOutForSingle.push_back({INF, -1});
            }
        
        } else {
            minOutForSingle.push_back(out[0]);
            minInForSingle.push_back(in[0]);
        }

        file << "Минимальное входящее ребро " << minInForSingle.back().second << "→" << j 
        << " имеет вес " << minInForSingle.back().first << '\n';
        file << "Минимальное выходящее ребро " << j << "→" << minOutForSingle.back().second 
        << " имеет вес " << minOutForSingle.back().first << "\n\n";
    }

    double L = (minIn + minOut + sum(minInForSingle) + sum(minOutForSingle)) / 2.0; //Оценка по всем кускам (большой и одиночные)

    file << "Нижняя оценка стоимости остатка решения: " << L << '\n';

    return L;
}

int main(){
    int n;
    int start;  //Стартовая вершина
    std::string line;
    std::vector<std::vector<int>> weights;  //Массив весов ребер
    std::ofstream file("output_bsa.txt");   //Файл для записи промежуточных результатов

    std::cin >> n >> start;
    std::cin.ignore();

    extractInputMatrix(n, weights);
    
    int pathLength = 0;
    std::vector<int> path = {start};
    std::vector<bool> visited(n, false);    //Массив посещенных вершин
    visited[start] = true;  //Начинаем со старта и отмечаем его посещенным

    while (path.size() != n-1){
        int minVertex = -1;
        double minMark = INF;   //Минимальная общая оценка
        int addedWeight = INF;
        int currentEnd = path[path.size() - 1];

        for (int i = 0; i < n; ++i){
            if (visited[i]) continue;    //Если вершина уже посещена

            int weight = weights[currentEnd][i];

            if (weight == 0) continue;

            double mark = lowerBound(start, i, n, weights, visited, file) + weight;   //Оценка для добавления вершины-кандидата i в конец

            file << "Полная оценка: " << mark << '\n';
            file << "---------------------------------------------------------------------\n";

            if (mark < minMark){
                minMark = mark;
                minVertex = i;
                addedWeight = weight;
            }
        }

        // После выбора вершины
        if (minMark >= INF/2){
            file << "Путь не существует!\n";
            std::cout << "no path\n";
            break;
        } else {
            file << "Выбрана дуга " << currentEnd << "→" << minVertex 
            << " вес=" << addedWeight << " оценка=" << minMark << std::endl;
            file << "======================================================================\n";

            pathLength += addedWeight;
            path.push_back(minVertex);
            visited[minVertex] = true;
        }
    }

    if (path.size() == n-1){
        //Отдельно записываем последнюю вершину

        int notVisited;
        for (int i = 0; i < n; ++i){
            if (!visited[i]){
                notVisited = i;
                break;
            }
        }

        int lastWeight = weights[path[path.size() - 1]][notVisited];
        pathLength += lastWeight;
        path.push_back(notVisited);

        file << "Оставшаяся вершина: " << notVisited << '\n';
        file << "Ребро в старт " << notVisited << "→" << start << " = " << weights[path[path.size() - 1]][start] << '\n';
        file << "Ребро в конец " << notVisited << "→" << path[path.size() - 2] << " = " << lastWeight << "\n\n";

        pathLength += weights[path[path.size() - 1]][start];  //Замыкающее ребро из конечной вершины в стартовую

        file << "Итог\n" << "Минимальный вес: " << pathLength << '\n' << "Путь: ";
    
        std::cout << pathLength << '\n';
        for (int i = 0; i < n; ++i){
            file << path[i] << ' ';
            std::cout << path[i] << ' ';
        }
        file << start << '\n';
        std::cout << start << '\n';
    } 

    file.close();
    return 0;
}