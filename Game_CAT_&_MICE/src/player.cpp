#include "../include/player.hpp"

Player::Player():health(10), score(0), damage(typeOfFight::CLOSE), coins(0), 
    coordinates(std::make_pair(0, 0)), moveAbility(1), hand(Hand(3)){}

int Player::getHealth() const{
    return health;
}

void Player::setHealth(int newHealth){
    health = newHealth;
}

int Player::getScore() const{
    return score;
}

void Player::setScore(int newScore){
    score = newScore;
}

typeOfFight Player::getDamage() const{
    return damage;
}

void Player::setDamage(typeOfFight newDamage){
    damage = newDamage;
}

std::pair<int, int> Player::getCoordinates() const{
    return coordinates;
}

void Player::setCoordinates(int x, int y){
    coordinates = std::make_pair(x, y);
}

bool Player::getMoveAbility() const{
    return moveAbility;
}

void Player::setMoveAbility(bool value){
    moveAbility = value;
}

int Player::getCoins() const{
    return coins;
}

void Player::setCoins(int newCoins){
    coins = newCoins;
}

bool Player::hasSpell(int index){
    return hand.hasSpell(index);
}

spellType Player::getSpellName(int index){
    return hand.getNameSpellCard(index);
}

int Player::getHandSize() const{
    return hand.getSize();
}

bool Player::isAlive(){
    return health > 0;
}

bool Player::selectCombatMode(typeOfFight mode){
    if (damage != mode){
        damage = mode;
        return 1;
    }
    return 0;
}

bool Player::buySpell(){
    if (coins >= 5 && isAlive()){
        bool success = hand.addSpellCard();
        if (success){
            coins = coins - 5;
            return true;
        }
    }
    return false;
}

bool Player::applySpell(int spellIndex, Field& field, Enemy& enemy, std::pair<int, int> target){
    return hand.cast(spellIndex, field, *this, enemy, target);
}
