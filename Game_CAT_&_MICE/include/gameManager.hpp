#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "field.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemyTower.hpp"
#include "saveManager.hpp"

struct LevelAttributes{
    std::vector<spellType> spells;
    int enemyHP = 5;
    int playerHP = 10;
    int fieldWidth = 16;
    int fieldLength = 18;
    int spellDamageKoef = 1;
    int playerDamageKoef = 1;
    int playerImproveHP = 0;
    int enemyDamage = 1;
    int goalMoves = 30;
    int goalScore = 1;
    int cellSize = 48;
};

class GameManager{
    Player player;
    Enemy enemy;
    Field field;
    EnemyTower tower;
    int moves;

public:
    GameManager();
    GameManager(LevelAttributes levelParameters);
    Player getPlayer() const;
    void setPlayer(Player newPlayer);
    Enemy getEnemy() const;
    Field getField() const;
    std::pair<int, int> getTowerCoords() const;
    int getMoves() const;
    void setMoves(int newValue);

    void placeCharacter(character person);
    bool isEnemyInRange(int range);
    bool movePlayer(char direction);
    void moveEnemy();
    void attackEnemy();
    void attackPlayer();
    bool applySpell(int spellIndex, std::pair<int, int> target);
    void buySpell(int koef);
    bool towerAttack(Player& target);

    SaveData convertToSaveData();
    void unpackSaveData(SaveData data);
};

#endif