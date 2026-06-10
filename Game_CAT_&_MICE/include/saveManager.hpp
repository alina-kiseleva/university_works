#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

struct SaveData{
    int fieldLength;
    int fieldWidth;
    std::vector<int> cellTypes;
    std::vector<int> cellCharacters;
    std::vector<int> cellDamages;
    
    int playerHealth;
    int playerCombatType;
    int playerDamage;
    int playerDamageKoef;
    int playerImproveHP;
    int coins;
    int score;
    bool playerMoveAbility;
    std::pair<int, int> playerCoordinates;
    std::vector<int> spellTypes;
    int spellsKoef;

    int enemyHealth;
    int enemyDamage;
    std::pair<int, int> enemyCoordinates;

    std::pair<int, int> towerCoordinates;

    int moves;
    int gameCondition;
    int goalMoves;
    int goalScore;
    int cellSize;

    int hash;
};

class SaveManager{
    std::string filePath = "../save.json";
public:
    bool saveToJson(SaveData saveData);
    bool loadFromJson(SaveData& saveData);
    bool checkSaveData(SaveData data);
    int makeHash(SaveData data);
    int mix(int hash, int value);
};

#endif