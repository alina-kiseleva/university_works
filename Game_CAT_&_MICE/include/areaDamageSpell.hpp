#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "iSpell.hpp"

class AreaDamageSpell: public ISpell{
    int range;
    int damage;
    int koef;

public:
    AreaDamageSpell(int k);
    spellType name() override;
    bool use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target, bool towerUse) override;
};

#endif