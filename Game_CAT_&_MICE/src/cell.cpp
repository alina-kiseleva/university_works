#include "../include/cell.hpp"

Cell::Cell():type(typeOfCell::DEFAULT), person(character::NOBODY), damage(0){}

Cell::Cell(typeOfCell type):type(type), person(character::NOBODY), damage(0){}

void Cell::setType(typeOfCell newType){
    type = newType;
}

void Cell::setCharacter(character newPerson){
    person = newPerson;
}

typeOfCell Cell::getType() const{
    return type;
}

character Cell::getCharacter() const{
    return person;
}

void Cell::setDamage(int newDamage){
    damage = newDamage;
}

int Cell::getDamage() const{
    return damage;
}