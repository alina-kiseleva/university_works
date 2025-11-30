#ifndef ENEMY_H
#define ENEMY_H

#include <algorithm>

class Enemy{
    int health;
    int baseHealth;
    int damage;
    std::pair<int, int> coordinates;

public:
    Enemy(int h, int d);

    void setBaseHealth(int newHealth);
    int getBaseHealth() const;
    int getHealth() const;
    void setHealth(int newHealth);
    int getDamage() const;
    void setDamage(int newDamage);
    std::pair<int, int> getCoordinates() const;
    void setCoordinates(int x, int y);
    bool isAlive();
};

#endif