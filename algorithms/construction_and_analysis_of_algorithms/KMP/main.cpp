#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<int> kmp(std::string pattern, std::string text, std::ofstream& file){
    std::string workString = pattern + '#' + text;  //Рабочая строка, паттерн и текст разделены символом '#'
    int n = workString.size();
    int m = pattern.size();
    std::vector<int> pi(m, 0);  //Вектор префикс-функции
    std::vector<int> ans;   //Вектор для индексов вхождений  

    bool hashtagFlag = 0;   //Флаг для '#'
    bool printSearchTitle = 0;  //Флаг для вывода заголовка поиска паттерна в тексте
    int j = 0;  //Указатель для передвижения по паттерну и счетчик подходящих под паттерн символов

    file << "Pабочая строка (паттерн + '#' + текст): " << workString << '\n';
    file << "Начальный вид префикс-функции: [";
    for (int i = 0; i < m; ++i)
        file << pi[i] << ' ';
    file << "]\n";
    file << "---------------------------------------------------\n";
    file << "=== Начало работы алгоритма ===\n";
    file << "+++ Вычисление префикс-функции +++\n\n";

    for (int i = 1; i < n; ++i){
        if (printSearchTitle){
            file << "+++ Поиск паттерна в тексте +++\n\n";
            printSearchTitle = 0;
        }

        file << i << "-й символ рабочей строки: " << workString[i] << '\n';
        file << j << "-й символ паттерна: " << workString[j] << '\n';

        if (workString[i] == '#'){
            hashtagFlag = 1;
            printSearchTitle = 1;
        }

        if (!hashtagFlag)
            j = pi[i-1];  //Для вычисления значений префикс-функции  

        while (j > 0 && workString[i] != workString[j]){
            j = pi[j-1];    //Откат на предыдущую позицию из префикс-функции
            file << "Откат: " << "указатель префикса теперь на позиции " << j << '\n';
        }
        
        if (workString[i] == workString[j]){
            if (hashtagFlag)
                file << i - m - 1 << "-й символ текста и " << j << "-й символ паттерна совпадают\n";
            j++;    //Увеличение счетчика при совпадении символов
        }

        if (!hashtagFlag){
            pi[i] = j;  //Запись значения префикс-функции

            file << "Префикс-функция: [";
            for (int k = 0; k < m; ++k)
                file << pi[k] << ' ';
            file << "]\n";
        }

        if (j == m){
            file << "Найдено вхождение паттерна в текст на индексе: " << i - 2*m << '\n';
            
            ans.push_back(i - 2*m); //2*m т.к. переход к началу вхождения + паттерн есть в рабочей строке
            j = pi[m-1];    //Откат на конец префикс-функции
            
            file << "Откат: " << "указатель префикса теперь на позиции " << j << '\n';
        }

        file << "---------------------------------------------------\n";
    }

    return ans;
}

int isCycleShift(std::string pattern, std::string text, std::ofstream& file){
    if (pattern.size() != text.size()) return -1;

    file << "\n---------------------------------------------------\n";
    file << "=== Проверка на циклический сдвиг ===\n\n";

    std::vector<int> indexes = kmp(pattern, text+text, file);   //Поиск текста в двойной строке паттерна
    return (indexes.size()) ? indexes[0] : -1;
}

int main(){
    std::ofstream file("output.txt");
    std::string patternA;
    std::string textB;

    std::cout << "Введите строку-паттерн для поиска:\n";
    std::cin >> patternA;
    std::cin.ignore();

    std::cout << "Введите текст:\n";
    std::getline(std::cin, textB);

    file << "=== Начало программы ===\n\n";
    file << "Паттерн: " << patternA << '\n';
    file << "Текст: " << textB << '\n';
    file << "---------------------------------------------------\n";
    file << "=== Запуск алгоритма KMП ===\n\n";
    
    std::vector<int> indexes = kmp(patternA, textB, file);

    std::cout << "\nИндексы вхождений паттерна в текст: ";
    file << "=== Итог ===\n\n";
    file << "Индексы вхождений паттерна в текст: ";

    for (int i = 0; i < indexes.size(); ++i){
        std::cout << indexes[i] << ' ';
        file << indexes[i] << ' ';
    }
    
    int cycleShift = isCycleShift(textB, patternA, file);
    std::string cycleFlag = (cycleShift == -1) ? " не " : " ";

    std::cout << "\nСтрока «" << patternA << "»" << cycleFlag << "является циклическим сдвигом строки «" << textB << "»\n";
    file << "\nСтрока «" << patternA << "»" << cycleFlag << "является циклическим сдвигом строки «" << textB << "»\n";

    if (cycleShift != -1){
        std::cout << "Индекс первого вхождения: " << cycleShift << '\n';
        file << "Индекс первого вхождения: " << cycleShift << '\n';
    }
    
    //std::cout  << '\n' << cycleShift << '\n';
    std::cout << "\nПромежуточные данные в файле output.txt\n";

    file.close();

    return 0;
}