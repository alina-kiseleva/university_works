#include "../include/enemy.hpp"

Enemy::Enemy(int h, int d):health(h), damage(d), coordinates(std::make_pair(-1, -1)){
    baseHealth = h;
}

int Enemy::getBaseHealth() const{
    return baseHealth;
}

void Enemy::setBaseHealth(int newHealth){
    baseHealth = newHealth;
}

int Enemy::getHealth() const{
    return health;
}

void Enemy::setHealth(int newHealth){
    health = newHealth;
}

int Enemy::getDamage() const{
    return damage;
}

void Enemy::setDamage(int newDamage){
    damage = newDamage;
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