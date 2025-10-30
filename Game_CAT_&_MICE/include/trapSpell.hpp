#ifndef TRAP_SPELL_H
#define TRAP_DAMAGE_SPELL_H

#include "iSpell.hpp"

class TrapSpell: public ISpell{
    int damage;

public:
    TrapSpell();
    spellType name() override;
    bool use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target) override;
    bool use(Player& target, std::pair<int, int> tower) override;
};

#endif