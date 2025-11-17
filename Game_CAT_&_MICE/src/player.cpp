#include "../include/player.hpp"

Player::Player(int health) : health(health), score(0), combatType(typeOfFight::CLOSE), coins(0), 
    coordinates(std::make_pair(0, 0)), moveAbility(1), hand(Hand(4)){
        damage = static_cast<int>(combatType);
    }

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

typeOfFight Player::getCombatType() const{
    return combatType;
}

void Player::setDamage(int newDamage){
    damage = newDamage;
}

int Player::getDamage() const{
    return damage;
}

void Player::setCombatType(typeOfFight newType){
    combatType = newType;
    setDamage(static_cast<int>(combatType) * lvlDamageKoef);
}

int Player::getDamageKoef() const{
    return lvlDamageKoef;
}

void Player::setDamageKoef(int newKoef){
    lvlDamageKoef = newKoef;
    setDamage(static_cast<int>(combatType) * lvlDamageKoef);
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

int Player::getHandFullness() const{
    return hand.getCurrentSize();
}

bool Player::isAlive(){
    return health > 0;
}

bool Player::selectCombatMode(typeOfFight mode){
    if (combatType != mode){
        combatType = mode;
        damage = static_cast<int>(combatType) * lvlDamageKoef;
        return 1;
    }
    return 0;
}

bool Player::buySpell(int koef){
    if (coins >= 5 && isAlive()){
        bool success = hand.addRandomSpellCard(koef);
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

void Player::deleteRandomSpell(int quantity){
    hand.deleteRandomSpellCard(quantity);
}

void Player::clearHand(){
    hand.clearSpells();
}

void Player::addSpell(spellType type, int koef){
    hand.addConcreteSpellCard(type, koef);
}