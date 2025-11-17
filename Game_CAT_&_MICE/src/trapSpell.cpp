#include "../include/trapSpell.hpp"
#include "../include/player.hpp"
#include "../include/enemy.hpp"
#include "../include/field.hpp"

TrapSpell::TrapSpell(int k):koef(k){
    damage = koef;
}

spellType TrapSpell::name(){
    return spellType::TRAP_SPELL;
}

bool TrapSpell::use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target, bool towerUse){
    if (field.canMoveTo(target.first, target.second)){
        if (field.getCellCharacter(target.first, target.second) == character::NOBODY){
            field.setCellCharacter(target.first, target.second, character::TRAP);
            field.setCellDamage(target.first, target.second, damage);
            return 1;
        }
    }
    return 0;
}