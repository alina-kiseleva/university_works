#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "field.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemyTower.hpp"

class GameManager{
    Player player;
    Enemy enemy;
    Field field;
    EnemyTower tower;
    int moves;
    bool isFinished;

public:
    GameManager();
    Player getPlayer() const;
    void setPlayer(Player newPlayer);
    Enemy getEnemy() const;
    Field getField() const;
    EnemyTower getTower() const;
    int getMoves() const;
    void setMoves(int newValue);
    bool getIsFinished() const;
    void setIsFinished(bool condition);

    void placeCharacter(character person);
    bool isEnemyInRange(int range);
    void movePlayer(char direction);
    void moveEnemy();
    void attackEnemy();
    void attackPlayer();
    bool applySpell(int spellIndex, std::pair<int, int> target);
    void buySpell();
    bool towerAttack();
};

#endif