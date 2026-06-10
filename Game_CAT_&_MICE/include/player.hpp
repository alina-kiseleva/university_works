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
    typeOfFight combatType;
    int damage;
    int lvlDamageKoef = 1;
    std::pair<int, int> coordinates;
    Hand hand;

public:
    Player(int health);

    int getHealth() const;
    void setHealth(int newHealth);
    int getScore() const;
    void setScore(int newScore);
    int getDamage() const;
    void setDamage(int newDamage);
    typeOfFight getCombatType() const;
    void setCombatType(typeOfFight newType);
    void setDamageKoef(int newKoef);
    int getDamageKoef() const;
    std::pair<int, int> getCoordinates() const;
    void setCoordinates(int x, int y);
    bool getMoveAbility() const;
    void setMoveAbility(bool value);
    int getCoins() const;
    void setCoins(int newCoins);

    bool hasSpell(int index);
    spellType getSpellName(int index);
    int getHandSize() const;
    int getHandFullness() const;
    bool isAlive();
    bool selectCombatMode(typeOfFight mode);
    bool buySpell(int koef);
    bool applySpell(int spellIndex, Field& field, Enemy& enemy, std::pair<int, int> target);
    void clearHand();
    void addSpell(spellType type, int koef);
    void deleteRandomSpell(int quantity);
};

#endif