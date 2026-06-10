#include "../include/gameManager.hpp"

GameManager::GameManager():player(Player(10)), enemy(Enemy(5, 1)),
        field(Field(15, 15)), moves(0), tower(EnemyTower()){}

GameManager::GameManager(LevelAttributes lvlParameters)
    :   player(Player(lvlParameters.playerHP)), 
        enemy(Enemy(lvlParameters.enemyHP, lvlParameters.enemyDamage)),
        field(Field(lvlParameters.fieldLength, lvlParameters.fieldWidth)),
        moves(0), tower(EnemyTower()) {

    player.clearHand();
    for (int i = 0; i < lvlParameters.spells.size(); ++i){
        player.addSpell(lvlParameters.spells[i], lvlParameters.spellDamageKoef);
    }

    player.setDamageKoef(lvlParameters.playerDamageKoef);
}

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

std::pair<int, int> GameManager::getTowerCoords() const{
    return tower.getCoordinates();
}

int GameManager::getMoves() const{
    return moves;
}

void GameManager::setMoves(int newMoves){
    moves = newMoves;
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
        enemy.setHealth(enemy.getBaseHealth());
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
        int playerDamage = player.getDamage();
        int playerTypeDamage = static_cast<int>(player.getCombatType());

        if ((playerTypeDamage == 1 && isEnemyInRange(2)) || (playerTypeDamage == 2 && isEnemyInRange(1))){
            enemyHealth -= playerDamage;
            if (enemyHealth < 0) {
                enemyHealth = 0;
            }
            enemy.setHealth(enemyHealth);
            player.setCoins(player.getCoins() + playerDamage);

            if (enemyHealth <= 0){
                auto [enemyX, enemyY] = enemy.getCoordinates();
                field.setCellCharacter(enemyX, enemyY, character::NOBODY);
                player.setScore(player.getScore() + 1);
                placeCharacter(character::ENEMY);
            }
        }
    }
}

bool GameManager::movePlayer(char direction){
    bool isActed = false;
    if (player.isAlive()){
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
                if (player.getMoveAbility()){ 
                    field.setCellCharacter(playerX, playerY, character::NOBODY);
                    field.setCellCharacter(newX, newY, character::PLAYER);
                    player.setCoordinates(newX, newY);
                    ++moves;

                    if (field.getCellType(newX, newY) == typeOfCell::FREEZE){
                        player.setMoveAbility(0);
                    }
                } else ++moves;
                isActed = true;
            }
        }
    }
    return isActed;
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

        int distance = abs(enemyX - playerX) + abs(enemyY - playerY);
        if (distance == 1) {
            attackPlayer();
            return;
        }

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
                    field.setCellCharacter(enemyX, enemyY, character::NOBODY);
                    field.setCellCharacter(bestMove.first, bestMove.second, character::NOBODY);
                    field.setCellDamage(bestMove.first, bestMove.second, 0);
                    player.setScore(player.getScore() + 1);
                    placeCharacter(character::ENEMY);
                }
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

void GameManager::buySpell(int koef){
    bool success = player.buySpell(koef);
    if (success){
        ++moves;
    }
}

bool GameManager::towerAttack(Player& target){
    return tower.attackTarget(field, target, enemy);
}

SaveData GameManager::convertToSaveData(){
    SaveData data;

    data.fieldLength = field.getLength();
    data.fieldWidth = field.getWidth();

    for (int i = 0; i < data.fieldLength; ++i){
        for (int j = 0; j < data.fieldWidth; ++j){
            data.cellTypes.push_back((int)field.getCellType(i, j));
            data.cellCharacters.push_back((int)field.getCellCharacter(i, j));
            data.cellDamages.push_back((int)field.getCellDamage(i, j));
        }
    }

    data.playerHealth = player.getHealth();
    data.playerCombatType = (int)player.getCombatType();
    data.playerDamage = player.getDamage();
    data.playerDamageKoef = player.getDamageKoef();
    data.coins = player.getCoins();
    data.score = player.getScore();
    data.playerMoveAbility = player.getMoveAbility();
    data.playerCoordinates = player.getCoordinates();
    
    for (int i = 0; i < 4; ++i){
        if (player.hasSpell(i)){
            data.spellTypes.push_back((int)player.getSpellName(i));
        }
    }

    data.enemyHealth = enemy.getHealth();
    data.enemyDamage = enemy.getDamage();
    data.enemyCoordinates = enemy.getCoordinates();

    data.towerCoordinates = tower.getCoordinates();
    data.moves = moves;

    return data;
}

void GameManager::unpackSaveData(SaveData data){
    Field newField(data.fieldLength, data.fieldWidth);
    
    newField.restoreField(data.cellTypes, data.cellCharacters, data.cellDamages);
    field = newField;

    player.clearHand();
    for (int i = 0; i < data.spellTypes.size(); ++i){
        spellType type = static_cast<spellType>(data.spellTypes[i]);
        player.addSpell(type, data.spellsKoef);
    }

    player.setHealth(data.playerHealth);
    if (data.playerCombatType == 1 || data.playerCombatType == 2){
    player.setCombatType(static_cast<typeOfFight>(data.playerCombatType));
    }
    player.setDamage(data.playerDamage);
    player.setDamageKoef(data.playerDamageKoef);
    player.setCoins(data.coins);
    player.setScore(data.score);
    player.setMoveAbility(data.playerMoveAbility);
    player.setCoordinates(data.playerCoordinates.first, data.playerCoordinates.second);

    enemy.setHealth(data.enemyHealth);
    enemy.setBaseHealth(data.enemyHealth);
    enemy.setDamage(data.enemyDamage);
    enemy.setCoordinates(data.enemyCoordinates.first, data.enemyCoordinates.second);
    
    tower.setCoordinates(data.towerCoordinates.first, data.towerCoordinates.second);
    
    moves = data.moves;
}