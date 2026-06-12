#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

std::string printSequenceofActions(std::vector<std::vector<char>> actions){    //Функция восстановления последовательности действий,
    int i = actions.size() - 1;                                               //произведенных для преобразования строки A в строку B
    int j = actions[0].size() - 1;

    std::string sequence;

    while (i != 0 || j != 0){
        sequence += actions[i][j];
        if (actions[i][j] == 'I') j--;
        else if (actions[i][j] == 'D') i--;
        else if (actions[i][j] == 'T'){
            i--;
            j -= 2;
        } else {
            i--;
            j--;
        }
    }

    std::reverse(sequence.begin(), sequence.end());
    return sequence;
}

void printMatrices(std::vector<std::vector<int>>& distances, std::vector<std::vector<char>>& actions, std::ofstream& file){
    int aLength = distances.size();
    int bLength = distances[0].size();

    for (int i = 0; i < aLength; ++i){
        for (int j = 0; j < bLength; ++j){
            if (distances[i][j] && actions[i][j] || (i == 0 && j == 0)){
                file << '[' << distances[i][j] << ", ";
                
                if (i == 0 && j == 0) file << ' ' << ']'; 
                else file << actions[i][j] << ']'; 
            }
            
            if (j == bLength - 1) file << '\n';
        }
    }
}

int main(){
    std::ofstream file("output.txt");
    int costReplace, costInsert, costDelete, costReplaceOneToTwo;
    std::string A, B; 

    std::cout << "Введите через пробел стоимость операции замены, вставки, удаления и замены одного символа на два:\n";
    std::cin >> costReplace >> costInsert >> costDelete;
    std::cin >> costReplaceOneToTwo;

    std::cout << "Введите начальную строку для преобразования и строку, которую нужно получить:\n";
    std::cin >> A >> B;

    file << "-----Начальные данные-----\n";
    file << "Стоимость замены: " << costReplace << '\n';
    file << "Стоимость вставки: " << costInsert << '\n';
    file << "Стоимость удаления: " << costDelete << '\n';
    file << "Стоимость замены одного символа на два: " << costReplaceOneToTwo << '\n';
    file << "Цель: преобразовать строку «" << A << "» в строку «" << B << "» за минимальную стоимость.\n";
    file << "--------------------------\n";

    std::vector<std::vector<int>> distances(A.size() + 1, std::vector<int>(B.size() + 1));  //Массив расстояний между префиксами
    std::vector<std::vector<char>> actions(A.size() + 1, std::vector<char>(B.size() + 1));  //Массив действий с префиксами

    for (int i = 0; i <= A.size(); ++i){  //Базовый случай: заполнение первого столбца
        distances[i][0] = i * costDelete;
        actions[i][0] = 'D';
    }

    for (int j = 0; j <= B.size(); ++j){  //Базовый случай: заполнение первой строки
        distances[0][j] = j * costInsert;
        actions[0][j] = 'I';
    }

    file << "---Начальное состояние матриц расстояний и действий---\n";
    printMatrices(distances, actions, file);
    file << "------------------------------------------------------\n";

    for (int i = 1; i <= A.size(); ++i){    //Заполнение матрицы расстояний Левенштейна
        for (int j = 1; j <= B.size(); ++j){
            int compareSymb = (A[i-1] == B[j-1]) ? 0 : 1;   //i и j это индексы в матрице, -1 для перехода к индексам строк
            int deleteSymb = distances[i - 1][j] + costDelete;
            int insert = distances[i][j - 1] + costInsert;
            int replace = distances[i - 1][j - 1] + costReplace * compareSymb;    //Стоимость замены добавляется только в случае, если символы не совпадают 
            int replaceOneToTwo = (j >= 2) ? distances[i - 1][j - 2] + costReplaceOneToTwo : costReplaceOneToTwo * 100;

            file << "Стоимость вставки символа «" << B[j-1] << "»: " << insert << '\n';
            file << "Стоимость удаления символа «" << A[i-1] << "»: " << deleteSymb << '\n';
            file << "Стоимость замены символа «" << A[i-1] << "» на символ «" << B[j-1] << "»: " << replace << '\n';
            file << "Стоимость замены одного символа «" << A[i-1] << "» на два";
            
            if (j >= 2)  file << " «" << B.substr(j - 2, 2) << "»: " << replaceOneToTwo << '\n';
            else file << ": недоступно\n";

            distances[i][j] = std::min(std::min(deleteSymb, insert), std::min(replace, replaceOneToTwo));

            file << "Минимальную стоимость " << distances[i][j] << " имеет действие ";

            if (distances[i][j] == deleteSymb){    //Заполнение массива действий
                actions[i][j] = 'D'; 
                file << "УДАЛЕНИЯ\n";
            } else if (distances[i][j]== insert){
                actions[i][j] = 'I';
                file << "ВСТАВКИ\n";
            } else if (distances[i][j] == replace){
                if (compareSymb) actions[i][j] = 'R';
                else actions[i][j] = 'M';
                file << "ЗАМЕНЫ\n";
            } else if (distances[i][j] == replaceOneToTwo){
                actions[i][j] = 'T';
                file << "ЗАМЕНЫ ОДНОГО СИМВОЛА НА ДВА\n";
            }

            file << "\nТекущее состояние матрицы расстояний и действий:\n";
            printMatrices(distances, actions, file);
            file << "------------------------------------------------------\n";
        }
    }
 
    std::cout << "Минимальная стоимость: ";
    std::cout << distances[A.size()][B.size()] << '\n';
    std::string sequence = printSequenceofActions(actions);
    std::cout << "Последовательность действий: ";
    std::cout << sequence << '\n';
    std::cout << "Промежуточные данные в файле output.txt\n";

    file << "Итог:\nСтоимость преобразования строки «" << A << "» в строку «" << B << "»: " << distances[A.size()][B.size()] << '\n';
    file << "Последовательность действий (кратко):" << sequence << '\n';
    file << "Справка:\n";
    file << "R - замена\nM - совпадение (бездействие)\nI - вставка\nD - удаление\nT - замена одного символа на два\n";

    file.close();

    return 0;
}