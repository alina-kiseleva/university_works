#include <iostream>
#include <vector>
#include <fstream>
#include <string>      
#include <queue>    
#include <algorithm>

enum Alphabet{
    A, C, G, T, N
};

struct Node{
    int next[5];                        //Массив переходов
    int suffLink = -1;                  //Суффиксная ссылка
    int upLink = -1;                    //Конечная суффиксная ссылка
    bool isTerm = false;                
    std::vector<int> patternIDs;

    Node(){
        for (int i = 0; i < 5; ++i)
            next[i] = -1;
    }
};

struct PatternData{                     //Структура для хранения данных о паттерне для нахождения пересечений
    int start = -1;
    int end = -1;
    std::string pattern = "";
};

int getIndex(char symb){                //Преобразование символа в индекс алфавита
    switch(symb){
        case 'A': return Alphabet::A;
        case 'C': return Alphabet::C;
        case 'G': return Alphabet::G;
        case 'T': return Alphabet::T;
        case 'N': return Alphabet::N;
        default: return -1;
    }
}

char getChar(int id){                   //Перевод индекса в символ алфавита
    switch(id){
        case Alphabet::A: return 'A';
        case Alphabet::C: return 'C';
        case Alphabet::G: return 'G';
        case Alphabet::T: return 'T';
        case Alphabet::N: return 'N';
        default: return '?';
    }
}

void saveTransitionTable(Node node, std::ofstream& file){   //Сохранение таблицы переходов для ноды в файл
    for (int i = 0; i < 5; ++i)
        file << node.next[i] << ' ';
    file << '\n';
}

std::vector<Node> makeTrie(std::vector<std::string> patterns, std::ofstream& file){      //Функция построения бора
    std::vector<Node> trie;
    trie.push_back(Node());                                                 //Вставка пустого узла для корня

    for (int i = 0; i < patterns.size(); ++i){
        int currentVertex = 0;
        std::string pattern = patterns[i];
        file << "\nВставка паттерна «" << pattern << "»:\n";

        for (int j = 0; j < pattern.size(); ++j){
            int index = getIndex(pattern[j]);                               //Получение индекса символа в алфавите

            if (trie[currentVertex].next[index] == -1){                     //Построение ребра по данному символу, если его еще нет
                trie.push_back(Node());
                trie[currentVertex].next[index] = trie.size() - 1;          //Запись связи между нодами по получившемуся ребру в таблице переходов по данному символу
                
                file << "Добавлено ребро:\n"; 
                file << "\tИз вершины " << currentVertex << " в вершину " << trie.size() - 1 << '\n';
                file << "\tСимвол ребра: " << getChar(index) << '\n';
            }

            file << "Значения таблицы переходов для узла " << currentVertex << ": ";
            saveTransitionTable(trie[currentVertex], file);
            
            currentVertex = trie[currentVertex].next[index];                //Переход к следующей вершине из таблицы переходов
        }

        trie[currentVertex].isTerm = true;
        trie[currentVertex].patternIDs.push_back(i);                        //Запись значения паттерна, который закончился на данной ноде

        file << "* Вершина " << currentVertex << " терминальная\n";
    }

    return trie;
}

void buildAutomat(std::vector<Node>& trie, std::ofstream& file){     //Функция построения автомата
    std::queue<int> queueBfs;

    file << "Обработка детей корня:\n";

    for (int i = 0; i < 5; ++i){                //Подготовка детей корня
        if (trie[0].next[i] != -1){
            int child = trie[0].next[i];
            trie[child].suffLink = 0;
            trie[child].upLink = 0;
            queueBfs.push(child);

            file << "\tИндекс узла ребенка: " << trie[0].next[i] << '\n';
            file << "\tСимвол ребра: " << getChar(i) << '\n';
            file << "\tСуффиксная ссылка указывает на узел " << trie[child].suffLink << '\n';
            file << "\tКонечная суффиксная ссылка указывает на узел " << trie[child].upLink << '\n';
        
        } else {
            trie[0].next[i] = 0;                //Если символ не ребенок, ссылаемся на узел корня в таблице переходов
        }
    }

    file << "Значения таблицы переходов для узла 0: ";
    saveTransitionTable(trie[0], file);
    file << "\n--- Обход бора в ширину ---\n\n";

    while(queueBfs.size()){                     //Обход бора в ширину
        int currentNode = queueBfs.front();
        queueBfs.pop();

        for (int i = 0; i < 5; ++i){
            if (trie[currentNode].next[i] != -1){
                int child = trie[currentNode].next[i];
                trie[child].suffLink = trie[trie[currentNode].suffLink].next[i];            //Построение суффиксной ссылки для ребенка ноды

                int link = trie[child].suffLink;
                trie[child].upLink = (trie[link].isTerm) ? link : trie[link].upLink;        //Построение конечной суффиксной ссылки

                file << "Индекс узла ребенка: " << trie[currentNode].next[i] << '\n';
                file << "Символ ребра: " << getChar(i) << '\n';
                file << "Суффиксная ссылка указывает на узел " << trie[child].suffLink << '\n';
                file << "Конечная суффиксная ссылка указывает на узел " << trie[child].upLink << '\n';
                file << "--------------------------------------------------------\n";

                queueBfs.push(child);
            
            } else {
                file << "Ребро с символом " << getChar(i) << " отсутствует\n";
                file << "Переход по суффиксной ссылке к узлу " << trie[currentNode].suffLink << '\n';
                file << "--------------------------------------------------------\n";

                trie[currentNode].next[i] = trie[trie[currentNode].suffLink].next[i];       //Если у текущего узла нет i-го потомка - переход по суффиксной ссылке и поиск в другой подстроке
            }
        }
    }
}

std::vector<std::vector<int>> ahoCorasik(std::string text, std::vector<std::string> patterns, int& cntNodes, std::ofstream& file){       //Функция поиска паттернов по предварительно построенному автомату
    file << "\n=== ПОСТРОЕНИЕ БОРА ===\n";
    std::vector<Node> trie = makeTrie(patterns, file);
    
    file << "\n=== ПОСТРОЕНИЕ АВТОМАТА ===\n\n";
    buildAutomat(trie, file);
   
    cntNodes = trie.size();
    file << "\n*** УЗЛОВ В АВТОМАТЕ: " << cntNodes << " ***\n";
    file << "\n=== ПОИСК ПАТТЕРНОВ В ТЕКСТЕ ===\n\n";

    std::vector<std::vector<int>> result(patterns.size());
    int j = 0;                                                   //Указатель для прохода по бору
    file << "СТАРТ: указатель бора в узле " << j << "\n\n";

    for (int i = 0; i < text.size(); ++i){
        int textSymb = getIndex(text[i]);
        file << i << "-й символ текста: " << text[i] << '\n';

        if (textSymb == -1){
            j = 0;
            continue;
        }

        file << "ДО перехода указатель бора в узле " << j << '\n';;
        file << "Переход по символу " << getChar(textSymb) << '\n';

        j = trie[j].next[textSymb];         //Проход по бору в соответствие с символами текста
        int check = j;

        file << "ПОСЛЕ перехода указатель бора в узле " << j << '\n';
        file << "--- Проверка на окончание паттерна ---\n";
        bool endPattern = false;                            //Флаг для проверки окончания паттерна на данном узле бора

        while (check > 0){
            if (trie[check].isTerm){                        //Если нода терминальная, то добавляем индекс вхождения паттерна(-ов) в финальный массив
                file << "Узел " << check << " терминальный\n";
                file << "На нем заканчиваются паттерны:\n";
                endPattern = true;

                for (int id : trie[check].patternIDs){
                    int textStartIndex = i - patterns[id].size() + 1;
                    result[id].push_back(textStartIndex);

                    file << "- " << patterns[id] << '\n';
                }
            }
            check = trie[check].upLink;
        }

        if (!endPattern) file << "Паттерн не заканчивается в этом узле\n";
        file << "--------------------------------------------------------\n";
    }

    return result;
}

bool checkJoker(std::string pattern, char joker){
    std::string invalidJoker(pattern.size(), joker);    //Проверка на валидность: паттерн не может состоять только из символов-джокеров
    return (pattern == invalidJoker) ? false : true;
}

bool checkPatterns(std::vector<std::string> patterns, char joker = '\0'){      //Функция проверки паттернов на наличие символов, не содержащихся в алфавите
    bool isValid = true;

    for (int i = 0; i < patterns.size(); ++i){
        for (int symb : patterns[i]){
            if (joker != '\0' && symb == joker) continue;

            int index = getIndex(symb);
            if (index == -1){
                isValid = false;
                break;
            }
        }
        if (!isValid) break;
    }
    return isValid;
}

std::vector<int> searchJokerPattern(std::string text, std::string pattern, char joker, int& cntNodes, std::ofstream& file){
    std::vector<int> tokensPositions;
    std::vector<std::string> tokens;
    std::string token = "";

    file << "\n=== ПОИСК ПАТТЕРНА В СТРОКЕ ===\n\n";
    file << "--- Разбиение паттерна на токены по символу джокера ---\n";
    
    for (int i = 0; i < pattern.size(); ++i){       //Разбиение паттерна на токены по символу-джокеру
        file << "Текущий символ паттерна: " << pattern[i] << '\n';

        if (pattern[i] != joker){
            if (token.empty()){
                tokensPositions.push_back(i);
                file << "Новый токен на позиции " << i << '\n';
            }

            token += pattern[i];
            file << "К токену добавлен символ, текущий вид: " << token << '\n';

        } else {
            if (!token.empty()){
                file << "Достигнут конец токена " << token << '\n';
                tokens.push_back(token);
                token = "";
            }
        }
    }

    if (!token.empty()){                 //В случае, если паттерн не заканчивается джокером
        tokens.push_back(token);
        file << "Последний токен: " << token << '\n';
    }
    
    file << "\n=== МАССИВ ТОКЕНОВ: ";
    for (int i = 0; i < tokens.size(); ++i){
        if (i != tokens.size() - 1) file << tokens[i] << ", "; 
        else file << tokens[i] << '\n';
    }

    file << "\n=== ПОИСК ТОКЕНОВ В ТЕКСТЕ ===\n";

    std::vector<std::vector<int>> result = ahoCorasik(text, tokens, cntNodes, file);      //Поиск токенов паттерна в тексте: по номерам токенов лежат индексы их вхождений в текст
    std::vector<int> counts(text.size(), 0);                                        //Массив счетчиков, где на каждому индексу текста соответствует количество токенов, указывающих на начальную позицию паттерна

    file << "\n=== ВЫЧИСЛЕНИЕ ИНДЕКСОВ НАЧАЛА ПАТТЕРНА В ТЕКСТЕ ===\n\n";

    for (int id = 0; id < result.size(); ++id){
        for (int textIndex : result[id]){
            int patternStartIndex = textIndex - tokensPositions[id];                //Вычисление начала паттерна по индексу каждого токена
            file << "Токен «" << tokens[id] << "» считает, что паттерн начинается на индексе " << patternStartIndex << '\n';

            if (patternStartIndex >= 0)
                counts[patternStartIndex]++;                    //Увеличение счетчика по полученному индеску начала паттерна
        }
    }

    std::vector<int> patternPositions;
    int maxPossibleStart = text.size() - pattern.size();

    for (int i = 0; i <= maxPossibleStart; ++i){
        if (counts[i] == tokens.size())                         //Если на индексе количество счетчиков = количеству токенов, то он является началом паттерна
            patternPositions.push_back(i);
    }

    return patternPositions;
}

void findIntersections(std::vector<PatternData>& allPatternsData, std::vector<std::vector<std::string>>& intersections){         //Функция поиска пересечений паттернов в тексте
    for (int i = 0; i < allPatternsData.size(); ++i){
        for (int j = i+1; j < allPatternsData.size(); ++j){
            if (allPatternsData[i].start <= allPatternsData[j].end && allPatternsData[j].start <= allPatternsData[i].end){
                intersections[allPatternsData[i].start].push_back(allPatternsData[i].pattern);
                intersections[allPatternsData[j].start].push_back(allPatternsData[j].pattern);
            }
        }
    }
}

int main(){
    std::ofstream file("output.txt");

    std::string text;
    std::vector<std::string> patterns;
    int n = 0;
    char joker;
    int cntNodes = 0;
    
    std::vector<PatternData> allPatternsData;                       //Массив для хранения данных обо всех встреченных в тексте паттернах
    std::vector<std::vector<std::string>> intersections;            //Массив для хранения пересечений: на индексах текста добавлены старты пересекающихсч паттернов
    int task = 0;

    std::cout << "Номер задания со Stepik (1 или 2): ";
    std::cin >> task;
    std::cin.ignore();

    std::cout << "Введите текст\n";
    std::getline(std::cin, text);

    intersections.resize(text.size());

    file << "=== ВХОДНЫЕ ДАННЫЕ ===\n\nТекст: " << text << '\n';

    if (task == 1){
        std::cout << "Введите количество паттернов\n";
        std::cin >> n;
        std::cin.ignore();

        std::cout << "Введите " << n << " паттернов\n";
        file << "Искомые паттерны:\n";
        patterns.resize(n);
        
        for (int i = 0; i < n; ++i){
            std::getline(std::cin, patterns[i]);
            file << '\t' << patterns[i] << '\n';
        }

        if (checkPatterns(patterns)){
            std::vector<std::vector<int>> result = ahoCorasik(text, patterns, cntNodes, file);        //Результирующий массив, где каждому номеру паттерна соответствует вектор из индексов в тексте, где он встречен
            std::vector<std::vector<int>> answer(text.size());             //Массив, где каждому индексу в тексте соответствует вектор номеров паттернов

            file << "\n=========== ИТОГ ===========\n";

            for (int id = 0; id < result.size(); ++id){                    //Преобразование массива <номер_паттерна индексы_текста> в <индекс_текста номера_паттернов>
                for (int textIndex : result[id]){                          
                    answer[textIndex].push_back(id);
                    file << "Паттерн «" << patterns[id] << "» начинается на индексе " << textIndex + 1 << '\n';
                }
            }

            std::cout << "Индекс_вхождения ID_паттерна\n";

            for (int textIndex = 0; textIndex < answer.size(); ++textIndex){  
                for (int id : answer[textIndex]){
                    std::cout << textIndex + 1 << ' ' << id + 1 << '\n';            //Отсортированный вывод вида <индекс_текста номер_паттерна>

                    PatternData patternData;                                        //Сохранение данных о паттерне: индексов начала и конца в тексте, значения паттерна
                    patternData.start = textIndex;
                    patternData.end = textIndex + patterns[id].size() - 1;
                    patternData.pattern = patterns[id];
                    allPatternsData.push_back(patternData);
                }
            }

            findIntersections(allPatternsData, intersections);           //Нахождение пересечений паттернов по получившемуся линейному массиву с данными о паттернах
       
        } else std::cout << "Ошибка: в паттерне встречен символ не из алфавита\n";

    } else if (task == 2){
        n = 1;                                                      //Паттерн с джокерами всегда один
        patterns.resize(n);

        std::cout << "Введите паттерн, содержащий джокеры\n";
        
        std::getline(std::cin, patterns[0]);
        file << "Искомый паттерн: " << patterns[0] << '\n';

        std::cout << "Введите символ-джокер\n";
        std::cin >> joker;
        file << "Символ-джокер: " << joker << '\n';

        bool isPatternValid = checkJoker(patterns[0], joker);       //Проверка корректности введенного паттерна

        if (isPatternValid && checkPatterns(patterns, joker)){
            std::vector<int> patternPositions = searchJokerPattern(text, patterns[0], joker, cntNodes, file);     //Поиск паттерна с джокерами: возвращен массив индексов вхождений паттерна в текст

            std::cout << "Позиции вхождений паттерна в текст:\n";
            file << "\n=========== ИТОГ ===========\n";

            for (int i = 0; i < patternPositions.size(); ++i){
                std::cout << patternPositions[i] + 1 << '\n';

                PatternData patternData;                            //Сохранение данных о паттерне: индексов начала и конца в тексте, значения паттерна
                patternData.start = patternPositions[i];
                patternData.end = patternPositions[i] + patterns[0].size() - 1;
                patternData.pattern = text.substr(patternPositions[i], patterns[0].size());
                allPatternsData.push_back(patternData);

                file << "Паттерн «" << patternData.pattern << "» начинается на индексе " << patternPositions[i] + 1 << '\n';
            }
            
            findIntersections(allPatternsData, intersections);

        } else std::cout << "Ошибка: недопустимый формат паттерна\n";
    }

    std::cout << "Количество узлов в автомате: " << cntNodes << '\n';
    std::cout << "Пересечения паттернов в строке:\n";
    std::cout << "Паттерн\tИндекс_вхождения\n";

    file << "\n=== ПЕРЕСЕЧЕНИЯ ПАТТЕРНОВ ===\n";

    for (int i = 0; i < intersections.size(); ++i){
        if (intersections[i].empty()) continue;

        std::sort(intersections[i].begin(), intersections[i].end());
        intersections[i].erase(std::unique(intersections[i].begin(), intersections[i].end()), intersections[i].end());  //Удаление дубликатов на индексе текста

        for (int j = 0; j < intersections[i].size(); ++j){
            std::cout << intersections[i][j] << ' ' << i + 1 << '\n';
            file << "Паттерн «" << intersections[i][j] << "» на индексе " << i + 1 << '\n';
        }
    }

    file.close();
    return 0;
}