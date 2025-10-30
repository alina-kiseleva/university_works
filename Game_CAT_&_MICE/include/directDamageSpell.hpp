#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "iSpell.hpp"

class DirectDamageSpell: public ISpell{
    int range;
    int damage;
    
public:
    DirectDamageSpell();
    spellType name() override;
    bool use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target) override;
    bool use(Player& target, std::pair<int, int> tower) override;
};

#endif