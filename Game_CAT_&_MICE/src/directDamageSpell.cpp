#include "../include/directDamageSpell.hpp"
#include "../include/player.hpp"
#include "../include/enemy.hpp"
#include "../include/field.hpp"

DirectDamageSpell::DirectDamageSpell(int k):range(3), koef(k){
    damage = 2 * koef;
}

spellType DirectDamageSpell::name(){
    return spellType::DIRECT_DAMAGE_SPELL;
}

bool DirectDamageSpell::use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target, bool towerUse){
    auto [playerX, playerY] = player.getCoordinates();
    int manhattan_dist = std::abs(playerX - target.first) + std::abs(playerY - target.second);

    if (manhattan_dist <= range){ 
        if (field.getCellCharacter(target.first, target.second) == character::ENEMY){
            enemy.setHealth(enemy.getHealth() - damage);

            if (!enemy.isAlive()){
                field.setCellCharacter(target.first, target.second, character::NOBODY);
            }
            return 1;
        }
        if (towerUse){
            player.setHealth(player.getHealth() - damage/2);
            return 1;
        }
    }
    return 0;
}