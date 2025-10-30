#include "../include/enemyTower.hpp"

EnemyTower::EnemyTower():towerPosition(std::make_pair(-1, -1)){}

bool EnemyTower::attackTarget(Player& target){
    DirectDamageSpell spell;
    return spell.use(target, towerPosition);
}

void EnemyTower::setCoordinates(int x, int y){
    towerPosition.first = x;
    towerPosition.second = y;
}

std::pair<int, int> EnemyTower::getCoordinates() const{
    return towerPosition;
}