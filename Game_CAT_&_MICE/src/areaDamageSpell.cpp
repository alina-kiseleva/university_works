#include "../include/areaDamageSpell.hpp"
#include "../include/player.hpp"
#include "../include/enemy.hpp"
#include "../include/field.hpp"

AreaDamageSpell::AreaDamageSpell(int k):range(4), koef(k){
    damage = koef;
}

spellType AreaDamageSpell::name(){
    return spellType::AREA_DAMAGE_SPELL;
}

bool AreaDamageSpell::use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target, bool towerUse){
    auto [playerX, playerY] = player.getCoordinates();
    int targetX = target.first;
    int targetY = target.second;
    int manhattan_dist = std::abs(playerX - targetX) + std::abs(playerY - targetY);
   
    if (manhattan_dist <= range){
        std::vector<std::pair<int, int>> square = {
            {targetX, targetY},
            {targetX, targetY + 1},
            {targetX + 1, targetY},
            {targetX + 1, targetY + 1}
        };

        for (auto position: square){
            if (field.canMoveTo(position.first, position.second) && 
                field.getCellCharacter(position.first, position.second) == character::ENEMY){
                    
                enemy.setHealth(enemy.getHealth() - damage);
                if (!enemy.isAlive()){
                    field.setCellCharacter(position.first, position.second, character::NOBODY);
                }
            }
        }
    } else {
        return false;
    }
    return true;
}