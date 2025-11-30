#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include "cell.hpp"
#include "player.hpp"
#include "enemy.hpp"

class Field{
    std::vector<std::vector<Cell*>> area;
    int length;
    int width;

public:
    Field(int length, int width);
    Field(const Field& field);
    Field(Field&& field);
    Field& operator = (const Field& field);
    Field& operator = (Field&& field);

    character getCellCharacter(int x, int y) const;
    void setCellCharacter(int x, int y, character newCharacter);
    typeOfCell getCellType(int x, int y) const;
    void setCellType(int x, int y, typeOfCell newType);
    void setCellDamage(int x, int y, int newDamage);
    int getCellDamage(int x, int y) const;
    int getLength() const;
    int getWidth() const;
    bool canMoveTo(int x, int y);
    void restoreField(
        std::vector<int> cellTypes, 
        std::vector<int> cellCharacters, 
        std::vector<int> cellDamages
    );
    ~Field();
};

#endif