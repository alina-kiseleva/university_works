#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "iSpell.hpp"

class DirectDamageSpell: public ISpell{
    int range;
    int damage;
    int koef;
    
public:
    DirectDamageSpell(int k);
    spellType name() override;
    bool use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target, bool towerUse) override;
};

#endif