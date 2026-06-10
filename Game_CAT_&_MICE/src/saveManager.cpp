#include "../include/saveManager.hpp"
#include <iostream>

bool SaveManager::saveToJson(SaveData saveData) {
    try {
        nlohmann::json jsonStorage = {
            {"fieldLength", saveData.fieldLength},
            {"fieldWidth", saveData.fieldWidth},
            {"cellTypes", saveData.cellTypes},
            {"cellCharacters", saveData.cellCharacters},
            {"cellDamages", saveData.cellDamages},

            {"playerHealth", saveData.playerHealth},
            {"playerCombatType", saveData.playerCombatType},
            {"playerDamage", saveData.playerDamage},
            {"playerDamageKoef", saveData.playerDamageKoef},
            {"playerImproveHealth", saveData.playerImproveHP},
            {"playerCoins", saveData.coins},
            {"playerScore", saveData.score},
            {"playerMoveAbility", saveData.playerMoveAbility},
            {"playerCoordinates", {saveData.playerCoordinates.first, saveData.playerCoordinates.second}},
            {"playerSpellTypes", saveData.spellTypes},
            {"playerSpellsKoef", saveData.spellsKoef},
            
            {"enemyHealth", saveData.enemyHealth},
            {"enemyDamage", saveData.enemyDamage},
            {"enemyCoordinates", {saveData.enemyCoordinates.first, saveData.enemyCoordinates.second}},
            
            {"towerCoordinates", {saveData.towerCoordinates.first, saveData.towerCoordinates.second}},
            
            {"moves", saveData.moves},
            {"gameCondition", saveData.gameCondition},
            {"goalMoves", saveData.goalMoves},
            {"goalScore", saveData.goalScore},
            {"cellSize", saveData.cellSize},

            {"hash", saveData.hash}
        };
        
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filePath << std::endl;
            return false;
        }
        
        file << jsonStorage.dump(4);
        file.close();
        
        std::cout << "Game saved to: " << filePath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Save error: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadFromJson(SaveData& saveData) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for reading: " << filePath << std::endl;
            return false;
        }
        
        nlohmann::json jsonStorage;
        file >> jsonStorage;
        file.close();

        saveData.fieldLength = jsonStorage.at("fieldLength").get<int>();
        saveData.fieldWidth = jsonStorage.at("fieldWidth").get<int>();
        saveData.cellTypes = jsonStorage.at("cellTypes").get<std::vector<int>>();
        saveData.cellCharacters = jsonStorage.at("cellCharacters").get<std::vector<int>>();
        saveData.cellDamages = jsonStorage.at("cellDamages").get<std::vector<int>>();
        
        saveData.playerHealth = jsonStorage.at("playerHealth").get<int>();
        saveData.playerCombatType = jsonStorage.at("playerCombatType").get<int>();
        saveData.playerDamage = jsonStorage.at("playerDamage").get<int>();
        saveData.playerDamageKoef = jsonStorage.at("playerDamageKoef").get<int>();
        saveData.playerImproveHP = jsonStorage.at("playerImproveHealth").get<int>();
        saveData.coins = jsonStorage.at("playerCoins").get<int>();
        saveData.score = jsonStorage.at("playerScore").get<int>();
        saveData.playerMoveAbility = jsonStorage.at("playerMoveAbility").get<bool>();
        saveData.spellsKoef = jsonStorage.at("playerSpellsKoef").get<int>();
        
        auto playerCoords = jsonStorage.at("playerCoordinates").get<std::vector<int>>();
        saveData.playerCoordinates = {playerCoords[0], playerCoords[1]};
        saveData.spellTypes = jsonStorage.at("playerSpellTypes").get<std::vector<int>>();
        
        saveData.enemyHealth = jsonStorage.at("enemyHealth").get<int>();
        saveData.enemyDamage = jsonStorage.at("enemyDamage").get<int>();
        
        auto enemyCoords = jsonStorage.at("enemyCoordinates").get<std::vector<int>>();
        saveData.enemyCoordinates = {enemyCoords[0], enemyCoords[1]};
        
        auto towerCoords = jsonStorage.at("towerCoordinates").get<std::vector<int>>();
        saveData.towerCoordinates = {towerCoords[0], towerCoords[1]};
        
        saveData.moves = jsonStorage.at("moves").get<int>();
        saveData.gameCondition = jsonStorage.at("gameCondition").get<int>();
        saveData.goalMoves = jsonStorage.at("goalMoves").get<int>();
        saveData.goalScore = jsonStorage.at("goalScore").get<int>();
        saveData.cellSize = jsonStorage.at("cellSize").get<int>();
        
        saveData.hash = jsonStorage.at("hash").get<int>();

        std::cout << "Game loaded from: " << filePath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Load error: " << e.what() << std::endl;
        return false;
    }
}

int SaveManager::makeHash(SaveData data){
    int hash = 0;
    
    hash = mix(hash, data.fieldLength);
    hash = mix(hash, data.fieldWidth);
    hash = mix(hash, data.cellSize);
    hash = mix(hash, data.playerHealth);
    hash = mix(hash, data.playerCombatType);
    hash = mix(hash, data.playerDamage);
    hash = mix(hash, data.playerDamageKoef);
    hash = mix(hash, data.playerImproveHP);
    hash = mix(hash, data.coins);
    hash = mix(hash, data.score);
    hash = mix(hash, data.playerMoveAbility);
    hash = mix(hash, data.playerCoordinates.first);
    hash = mix(hash, data.playerCoordinates.second);
    hash = mix(hash, data.spellsKoef);
    hash = mix(hash, data.enemyHealth);
    hash = mix(hash, data.enemyDamage);
    hash = mix(hash, data.enemyCoordinates.first);
    hash = mix(hash, data.enemyCoordinates.second);
    hash = mix(hash, data.towerCoordinates.first);
    hash = mix(hash, data.towerCoordinates.second);
    hash = mix(hash, data.moves);
    hash = mix(hash, data.gameCondition);
    hash = mix(hash, data.goalMoves);
    hash = mix(hash, data.goalScore);

    for (int value : data.cellTypes){
        hash = mix(hash, value);
    }
    for (int value : data.cellDamages){
        hash = mix(hash, value);
    }
    for (int value : data.cellCharacters){
        hash = mix(hash, value);
    }
    for (int value : data.spellTypes){
        hash = mix(hash, value);
    }
    
    return hash;
}

int SaveManager::mix(int hash, int value){
    return ((hash << 5) + hash) + value;
}

bool SaveManager::checkSaveData(SaveData data){
    return data.hash == makeHash(data);
}
