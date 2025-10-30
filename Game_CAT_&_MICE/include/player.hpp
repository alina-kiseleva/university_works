#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
#include "hand.hpp"

enum class typeOfFight{
    CLOSE = 2,
    FAR = 1
};

class Player{
    int health;
    int score;
    int coins;
    bool moveAbility;
    typeOfFight damage;
    std::pair<int, int> coordinates;
    Hand hand;

public:
    Player();

    int getHealth() const;
    void setHealth(int newHealth);
    int getScore() const;
    void setScore(int newScore);
    typeOfFight getDamage() const;
    void setDamage(typeOfFight newDamage);
    std::pair<int, int> getCoordinates() const;
    void setCoordinates(int x, int y);
    bool getMoveAbility() const;
    void setMoveAbility(bool value);
    int getCoins() const;
    void setCoins(int newCoins);

    bool hasSpell(int index);
    spellType getSpellName(int index);
    int getHandSize() const;
    bool isAlive();
    bool selectCombatMode(typeOfFight mode);
    bool buySpell();
    bool applySpell(int spellIndex, Field& field, Enemy& enemy, std::pair<int, int> target);
};

#endif