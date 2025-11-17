#include "../include/enemyTower.hpp"

EnemyTower::EnemyTower():towerPosition(std::make_pair(-1, -1)){}

bool EnemyTower::attackTarget(Field& field, Player& target, Enemy& enemy){
    DirectDamageSpell spell(1);
    return spell.use(field, target, enemy, towerPosition, 1);
}

void EnemyTower::setCoordinates(int x, int y){
    towerPosition.first = x;
    towerPosition.second = y;
}

std::pair<int, int> EnemyTower::getCoordinates() const{
    return towerPosition;
}