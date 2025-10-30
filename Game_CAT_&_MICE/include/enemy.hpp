#ifndef ENEMY_H
#define ENEMY_H

#include <algorithm>

class Enemy{
    int health;
    int damage;
    std::pair<int, int> coordinates;

public:
    Enemy();

    int getHealth() const;
    void setHealth(int newHealth);
    int getDamage() const;
    std::pair<int, int> getCoordinates() const;
    void setCoordinates(int x, int y);
    bool isAlive();
};

#endif