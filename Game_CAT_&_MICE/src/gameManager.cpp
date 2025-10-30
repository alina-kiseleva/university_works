#include "../include/gameManager.hpp"

GameManager::GameManager():player(Player()), enemy(Enemy()), 
        field(Field(15, 15)), moves(0), tower(EnemyTower()){}

Player GameManager::getPlayer() const{
    return player;
}

void GameManager::setPlayer(Player newPlayer){
    player = newPlayer;
}

Enemy GameManager::getEnemy() const{
    return enemy;
}

Field GameManager::getField() const{
    return field;
}

EnemyTower GameManager::getTower() const{
    return tower;
}

int GameManager::getMoves() const{
    return moves;
}

void GameManager::setMoves(int newMoves){
    moves = newMoves;
}

bool GameManager::getIsFinished() const{
    return isFinished;
}

void GameManager::setIsFinished(bool condition){
    isFinished = condition;
}

void GameManager::placeCharacter(character person){
    int x, y;
    int length = field.getLength();
    int width = field.getWidth();

    std::random_device rd;
    std::mt19937 gen(rd());
    
    do{
        x = std::uniform_int_distribution<int>(1, length - 1)(gen);
        y = std::uniform_int_distribution<int>(1, width - 1)(gen);
    } while(field.getCellType(x, y) != typeOfCell::DEFAULT || 
        field.getCellCharacter(x, y) != character::NOBODY);

    if (person == character::ENEMY){
        field.setCellCharacter(x, y, character::ENEMY);
        enemy.setCoordinates(x, y);
        enemy.setHealth(5);
    } else if (person == character::ENEMY_TOWER){
        field.setCellCharacter(x, y, character::ENEMY_TOWER);
        tower.setCoordinates(x, y);
    }
}

bool GameManager::isEnemyInRange(int range){
    auto [playerX, playerY] = player.getCoordinates();
    auto [enemyX, enemyY] = enemy.getCoordinates();
    
    int distance = std::abs(playerX - enemyX) + std::abs(playerY - enemyY);
    return distance <= range;
}

void GameManager::attackEnemy(){
    int enemyHealth = enemy.getHealth();
    
    if (enemyHealth > 0){
        int playerDamage = static_cast<int>(player.getDamage());

        if ((playerDamage == 1 && isEnemyInRange(2)) || (playerDamage == 2 && isEnemyInRange(1))){
            enemyHealth -= playerDamage;
            if (enemyHealth < 0) {
                enemyHealth = 0;
            }
            enemy.setHealth(enemyHealth);
            player.setCoins(player.getCoins() + playerDamage);
            ++moves;

            if (enemyHealth <= 0){
                auto [enemyX, enemyY] = enemy.getCoordinates();
                field.setCellCharacter(enemyX, enemyY, character::NOBODY);
                player.setScore(player.getScore() + 1);
                if (!isFinished){
                    placeCharacter(character::ENEMY);
                }
            }
        }
    }
}

void GameManager::movePlayer(char direction){
    if (player.isAlive() && player.getMoveAbility()){
        auto [playerX, playerY] = player.getCoordinates();

        int newX = playerX;
        int newY = playerY;

        switch(direction){
            case 'w': newX = playerX - 1; break;
            case 's': newX = playerX + 1; break;
            case 'a': newY = playerY - 1; break;
            case 'd': newY = playerY + 1; break;
        }

        if (field.canMoveTo(newX, newY) && field.getCellCharacter(newX, newY) != character::TRAP){
            if (field.getCellCharacter(newX, newY) != character::ENEMY){
                field.setCellCharacter(playerX, playerY, character::NOBODY);
                field.setCellCharacter(newX, newY, character::PLAYER);
                player.setCoordinates(newX, newY);
                ++moves;

                if (field.getCellType(newX, newY) == typeOfCell::FREEZE){
                    player.setMoveAbility(0);
                }
            }
        }
    } else if (!player.getMoveAbility()){
        ++moves;
    } else if (!player.isAlive()){
        isFinished = 1;
    }
}

void GameManager::attackPlayer(){
    int playerHealth = player.getHealth();
    if (playerHealth > 0){
        playerHealth -= enemy.getDamage();
        player.setHealth(playerHealth);
    }
}

void GameManager::moveEnemy(){
    if (enemy.isAlive()){
        auto [enemyX, enemyY] = enemy.getCoordinates();
        auto [playerX, playerY] = player.getCoordinates();

        std::vector<std::pair<int, int>> allMoves;
        for (auto [dx, dy] : std::vector<std::pair<int, int>>{{-1,0}, {1,0}, {0,-1}, {0,1}}){
            int newX = enemyX + dx;
            int newY = enemyY + dy;
            if (field.canMoveTo(newX, newY)){
                allMoves.push_back({newX, newY});
            }
        }

        if (!allMoves.empty()){
            std::pair<int, int> bestMove = allMoves[0];
            int minDist = std::abs(allMoves[0].first - playerX) + std::abs(allMoves[0].second - playerY);

            for (auto& move : allMoves){
                int currentDist = std::abs(move.first - playerX) + std::abs(move.second - playerY);
                if (currentDist < minDist){
                    bestMove = move;
                    minDist = currentDist;
                }
            }
            
            if (field.getCellCharacter(bestMove.first, bestMove.second) == character::NOBODY){
                field.setCellCharacter(bestMove.first, bestMove.second, character::ENEMY);
                field.setCellCharacter(enemyX, enemyY, character::NOBODY);
                enemy.setCoordinates(bestMove.first, bestMove.second);
                
            } else if (field.getCellCharacter(bestMove.first, bestMove.second) == character::TRAP){
                enemy.setHealth(enemy.getHealth() - field.getCellDamage(bestMove.first, bestMove.second));
                if (enemy.isAlive()){
                    field.setCellCharacter(enemyX, enemyY, character::NOBODY);
                    enemy.setCoordinates(bestMove.first, bestMove.second);
                    field.setCellCharacter(bestMove.first, bestMove.second, character::ENEMY);
                    field.setCellDamage(bestMove.first, bestMove.second, 0);
                } else {
                    field.setCellCharacter(bestMove.first, bestMove.second, character::NOBODY);
                    field.setCellDamage(bestMove.first, bestMove.second, 0);
                    if (!isFinished){
                        placeCharacter(character::ENEMY);
                    }
                }
 
            } else if (field.getCellCharacter(bestMove.first, bestMove.second) == character::PLAYER){
                attackPlayer();
            }
        }
    }
}

bool GameManager::applySpell(int spellIndex, std::pair<int, int> target){
    if (player.isAlive()){
        bool isSpellApplied = player.applySpell(spellIndex, field, enemy, target);
        if (isSpellApplied){
            ++moves;
            return 1;
        }
    }
    return 0;
}

void GameManager::buySpell(){
    bool success = player.buySpell();
    if (success){
        ++moves;
    }
}

bool GameManager::towerAttack(){
    return tower.attackTarget(player);
}