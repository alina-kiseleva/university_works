#ifndef CELL_H
#define CELL_H

enum class typeOfCell{
    DEFAULT = 0,
    OBSTACLE,
    FREEZE
};

enum class character{
    NOBODY = 0,
    PLAYER,
    ENEMY,
    ENEMY_TOWER,
    TRAP
};

class Cell{
    typeOfCell type;
    character person;
    int damage;

public:
    Cell();
    Cell(typeOfCell type);

    void setType(typeOfCell newType);
    void setCharacter(character newPerson);
    typeOfCell getType() const;
    character getCharacter() const;
    void setDamage(int newDamage);
    int getDamage() const;
};

#endif