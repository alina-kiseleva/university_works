#include "../include/enemy.hpp"

Enemy::Enemy():health(5), damage(1), coordinates(std::make_pair(-1, -1)){}

int Enemy::getHealth() const{
    return health;
}

void Enemy::setHealth(int newHealth){
    health = newHealth;
}

int Enemy::getDamage() const{
    return damage;
}

std::pair<int, int> Enemy::getCoordinates() const{
    return coordinates;
}

void Enemy::setCoordinates(int x, int y){
    coordinates = std::make_pair(x, y);
}

bool Enemy::isAlive(){
    return health > 0;
}