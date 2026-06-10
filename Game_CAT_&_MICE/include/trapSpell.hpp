#ifndef TRAP_SPELL_H
#define TRAP_DAMAGE_SPELL_H

#include "iSpell.hpp"

class TrapSpell: public ISpell{
    int damage;
    int koef;

public:
    TrapSpell(int k);
    spellType name() override;
    bool use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target, bool useTower) override;
};

#endif