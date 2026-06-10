#ifndef ISPELL_H
#define ISPELL_H

#include <utility>

class Field;
class Player;
class Enemy;

enum class spellType{
    DIRECT_DAMAGE_SPELL = 0,
    AREA_DAMAGE_SPELL,
    TRAP_SPELL,
    NONE
};

class ISpell{
public:
    virtual spellType name() = 0;
    virtual bool use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target, bool towerUse) = 0;
};

#endif