#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "directDamageSpell.hpp"
#include <utility>

class EnemyTower{
    std::pair<int, int> towerPosition;

public:
    EnemyTower();
    bool attackTarget(Field& field, Player& target, Enemy& enemy);
    void setCoordinates(int x, int y);
    std::pair<int, int> getCoordinates() const;
};

#endif