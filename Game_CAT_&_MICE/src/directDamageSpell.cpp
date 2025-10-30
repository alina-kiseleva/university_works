#include "../include/directDamageSpell.hpp"
#include "../include/player.hpp"
#include "../include/enemy.hpp"
#include "../include/field.hpp"

DirectDamageSpell::DirectDamageSpell():range(3), damage(2){}

spellType DirectDamageSpell::name(){
    return spellType::DIRECT_DAMAGE_SPELL;
}

bool DirectDamageSpell::use(Field& field, Player& player, Enemy& enemy, std::pair<int, int> target){
    auto [playerX, playerY] = player.getCoordinates();
    int manhattan_dist = std::abs(playerX - target.first) + std::abs(playerY - target.second);

    if (manhattan_dist <= range && field.getCellCharacter(target.first, target.second) == character::ENEMY){
        enemy.setHealth(enemy.getHealth() - damage);

        if (!enemy.isAlive()){
            field.setCellCharacter(target.first, target.second, character::NOBODY);
        }
        return 1;
    }
    return 0;
}

bool DirectDamageSpell::use(Player& target, std::pair<int, int> tower){
    auto [playerX, playerY] = target.getCoordinates();
    int manhattan_dist = std::abs(playerX - tower.first) + std::abs(playerY - tower.second);
    
    if (manhattan_dist <= range){
        target.setHealth(target.getHealth() - damage/2);
        return 1;
    }
    return 0;
}