#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <numeric>
#include <fstream>

struct Square{  //Структура, хранящая координаты левого верхнего угла квадрата и длину его стороны
    int x, y, length;
    Square(int x, int y, int length) : x(x), y(y), length(length) {}
};

struct Frame{   //Структура, хранящая текущую расстановку квадратов на столешнице
    std::vector<Square> squares;
    std::vector<int> heights;   //Массив, хранящий профиль заполнения столбцов координат сверху вниз
    int lastSideSize;   //Последний доступный для использования размер стороны квадрата
};

int calcMaxSideSize(std::vector<int>& heights, int limit){  
    auto minIterator = std::min_element(heights.begin(), heights.end());
    int index = std::distance(heights.begin(), minIterator);
    int availableSize = 0;

    for (int i = index; i < heights.size(); ++i){
        if (heights[i] == heights[index])
            availableSize++;
        else break;
    }

    int res = std::min(static_cast<int>(heights.size() - heights[index]), availableSize);   //Минимум из свободной длины по X и по Y
    return std::min(res, limit);    //Сравнение с предельным значением
}

std::pair<int, int> calcCoords(std::vector<int>& heights){ 
    auto minIterator = std::min_element(heights.begin(), heights.end());
    int index = std::distance(heights.begin(), minIterator);

    return {index, heights[index]};
}

void pasteSquare(std::vector<int>& heights, int x, int size){   //Вставка квадратов посредством заполнения столбцов координат
    for (int i = x; i < x + size; ++i){
        heights[i] += size;
    }
}

bool isFilled(std::vector<int>& heights){
    for (int val : heights){
        if (val != heights.size())
            return false;
    }
    return true;
}

int main(){
    int n;
    std::cin >> n;
    int cntPush = 0;    //Счетчик количества операций вставки в стек (для исследования)
    std::ofstream file("output.txt");
    std::ofstream operationsResearch("research.txt", std::ios::app);
    
    if (n%2 == 0){
        int sideSize = n/2;
        std::cout << "Минимальное число квадратов, на которое можно разбить квадратную столешницу размера " << n << ": ";
        std::cout << 4 << '\n';
        std::cout << "Итоговое размещение квадратов:\n";

        std::cout << "Квадрат #1:\n - Координаты (x, y): (" << 1 << ", " << 1 << ")\n";
        std::cout << " - Длина стороны: " << sideSize << '\n';
        std::cout << "Квадрат #2:\n - Координаты (x, y): (" << 1 << ", " << 1 + sideSize << ")\n";
        std::cout << " - Длина стороны: " << sideSize << '\n';
        std::cout << "Квадрат #3:\n - Координаты (x, y): (" << 1 + sideSize << ", " << 1 << ")\n";
        std::cout << " - Длина стороны: " << sideSize << '\n';
        std::cout << "Квадрат #4:\n - Координаты (x, y): (" << 1 + sideSize << ", " << 1 + sideSize << ")\n";
        std::cout << " - Длина стороны: " << sideSize << '\n';
    
    } else {
        int bestCount = 2*n;    //Наименьшее количество квадратов
        Frame bestFrame;    //Лучший кадр
        std::stack<Frame> stackOfSolutions;     //Стэк из кадров, хранит расстановки квадратов для возможного отката
        
        Frame firstState;
        firstState.heights.resize(n, 0);

        if (n%3 != 0){      //Предварительная расстановка трех наибольших возможных квадратов по углам
            int firstBiggestSize = (n + 1) / 2;
            int secondBiggestSize = (n - 1) / 2;
            pasteSquare(firstState.heights, 0, firstBiggestSize);
            pasteSquare(firstState.heights, 0, secondBiggestSize);
            pasteSquare(firstState.heights, firstState.heights.size()-secondBiggestSize, secondBiggestSize);
            firstState.squares.push_back(Square(1, 1, firstBiggestSize));
            firstState.squares.push_back(Square(1, firstBiggestSize + 1, secondBiggestSize));
            firstState.squares.push_back(Square(firstBiggestSize + 1, 1, secondBiggestSize));
        }

        firstState.lastSideSize = -1;   //Кадр еще не рассматривался для вставки нового квадрата
        stackOfSolutions.push(firstState);

        file << "Рассмотрим заполнение профилей высот и размещение квадратов для кадров в стеке: \n";
        file << "! Кадры показываются в порядке просмотра в стеке (FILO)\n";

        while (!stackOfSolutions.empty()){
            Frame& currentState = stackOfSolutions.top();   //Очередной кадр расстановки квадратов
            int currentCnt = currentState.squares.size();   //Количество квадратов в текущей расстановке
            int maxWidth;   //Максимально возможная длина стороны квадрата на текущем шаге

            file << "Кадр №" << stackOfSolutions.size() << ":\n";
            file << "Заполненность столбцов: \n";

            for (int i = 0; i < currentState.heights.size(); ++i){
                file << currentState.heights[i] << ' ';
            }
            
            file << "\nКоличество квадратов: " << currentCnt << '\n';
            file << "Текущее размещение квадратов: \n";

            if (currentState.squares.empty()){
                file << "Нет ни одного квадрата.\n";
            } else {
                for (int i = 0; i < currentState.squares.size(); ++i){
                    file << "Квадрат #" << (i + 1) << ":\n";
                    file << " - Координаты (x, y): (" << currentState.squares[i].x << ", " << currentState.squares[i].y << ")\n";
                    file << " - Длина стороны: " << currentState.squares[i].length << '\n';
                }
            }

            file << "-----------------------------\n";

            if (isFilled(currentState.heights)){
                if (currentCnt <= bestCount){
                    bestFrame = currentState;
                    bestCount = currentState.squares.size();
                }
                stackOfSolutions.pop();
                continue;
            } 
            
            if (currentState.lastSideSize == -1){   
                maxWidth = calcMaxSideSize(currentState.heights, n - 1);
                currentState.lastSideSize = maxWidth;
            }

            if (currentCnt >= bestCount){
                stackOfSolutions.pop();
                continue;
            }

            std::pair<int, int> newSquareCoords = calcCoords(currentState.heights);     //Координаты верхнего левого угла вставляемого квадрата
            
            if (currentState.lastSideSize > 0){
                int currentWidth = currentState.lastSideSize;   //Длина стороны вставляемого квадрата
                currentState.lastSideSize--;    //Уменьшение длины для текущего состояния для возможного отката

                Frame nextState = currentState;
                pasteSquare(nextState.heights, newSquareCoords.first, currentWidth);

                Square newSquare = {newSquareCoords.first + 1, newSquareCoords.second + 1, currentWidth};
                nextState.squares.push_back(newSquare);
                nextState.lastSideSize = -1;
                stackOfSolutions.push(nextState);
                cntPush++;
            
            } else {
                stackOfSolutions.pop();
            }
        }

        std::cout << bestCount << '\n';
        std::cout << "Итоговое размещение квадратов:\n";
        
        for (int i = 0; i < bestFrame.squares.size(); i++){
            std::cout << "Квадрат #" << (i + 1) << ":\n";
            std::cout << " - Координаты (x, y): (" << bestFrame.squares[i].x << ", " << bestFrame.squares[i].y << ")\n";
            std::cout << " - Длина стороны: " << bestFrame.squares[i].length << '\n';
        }

        std::cout << "Промежуточный вывод содержимого кадров стека находится в файле output.txt\n";
    }

    operationsResearch << n << ' ' << cntPush << '\n';
    operationsResearch.close();
    file.close();
    
    return 0;
}