#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "iSpell.hpp"

class AreaDamageSpell: public ISpell{
    int range;
    int damage;

public:
    AreaDamageSpell();
    spellType name() override;
    bool use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target) override;
    bool use(Player& target, std::pair<int, int> tower) override;
};

#endif