#include "../include/game.hpp"
#include <iostream>

Game::Game() : gameStatus(GameStatus::MAIN_MENU), gameManager(GameManager()){}

void Game::start(){
    if (goToNextLVL){
        gameManager = GameManager(levelParameters);
        goToNextLVL = 0;
    } else {
        levelParameters = LevelAttributes();
        gameManager = GameManager();
    }
    gameManager.placeCharacter(character::ENEMY);
    gameManager.placeCharacter(character::ENEMY_TOWER);
}

void Game::checkGameState() {
    Player player = gameManager.getPlayer();
    int moves = gameManager.getMoves();
    int score = player.getScore();

    if (score >= levelParameters.goalScore && moves <= levelParameters.goalMoves) {
        gameStatus = GameStatus::VICTORY;
    } else if (moves > levelParameters.goalMoves || !player.isAlive()) {
        gameStatus = GameStatus::DEFEAT;
    }
}

bool Game::movePlayer(char direction){
    int movesBefore = gameManager.getMoves();
    gameManager.movePlayer(direction);
    return (movesBefore < gameManager.getMoves()) ? true : false;
}

bool Game::attack(){
    gameManager.attackEnemy();
    gameManager.setMoves(gameManager.getMoves() + 1);
    return true;
}

bool Game::changeCombat(typeOfFight type){
    Player player = gameManager.getPlayer();
    bool success = player.selectCombatMode(type);
    if (success){
        gameManager.setPlayer(player);
        gameManager.setMoves(gameManager.getMoves() + 1);
    }
    return success;
}

bool Game::buySpell(){
    int movesBefore = gameManager.getMoves();
    gameManager.buySpell(levelParameters.spellDamageKoef);
    return (movesBefore < gameManager.getMoves()) ? true : false;
}

bool Game::hasSpell(int position){
    Player player = gameManager.getPlayer();
    return player.hasSpell(position);
}

bool Game::changesAfterMove(){
    bool renderTowerOverlay = false;
    Player player = gameManager.getPlayer();
    gameManager.moveEnemy();
    
    if (!player.getMoveAbility()){
        if (!freezed){
            player.setMoveAbility(1);
            gameManager.setPlayer(player);
            freezed = 1;
        } else freezed -= 1;
    }

    player = gameManager.getPlayer();
    auto [towerX, towerY] = gameManager.getTowerCoords();
    auto [playerX, playerY] = player.getCoordinates();
    int distance = std::abs(towerX - playerX) + std::abs(towerY - playerY);

    if (distance <= 3) { // Радиус атаки башни
        renderTowerOverlay = true;
    }

    if (isTowerAttack && timeWithoutAttack > 0){
        --timeWithoutAttack;
    } else {
        isTowerAttack = gameManager.towerAttack(player);
        gameManager.setPlayer(player);
        timeWithoutAttack = 2;
    }

    return renderTowerOverlay;
}

bool Game::handleMouseClick(int mouseX, int mouseY, int selectedSpellCard){
    Field field = gameManager.getField();
    int length = field.getLength();
    int width = field.getWidth();
    int CELL_SIZE = levelParameters.cellSize;
    
    if (mouseX < width * CELL_SIZE) {
        int cellX = mouseY / CELL_SIZE;
        int cellY = mouseX / CELL_SIZE;
        
        if (cellX >= 0 && cellX < length && cellY >= 0 && cellY < width) {
            bool success = gameManager.applySpell(selectedSpellCard, {cellX, cellY});
            if (success) {
                if (!gameManager.getEnemy().isAlive()){
                    gameManager.placeCharacter(character::ENEMY);
                }
                gameManager.moveEnemy();
                checkGameState();
            }
            return true;
        }
    }
    return false;
}

bool Game::handleMenuClick(int mouseX, int mouseY){
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonSpacing = 20;

    switch (gameStatus) {
        case GameStatus::MAIN_MENU: {
            int totalHeight = 3 * buttonHeight + 2 * buttonSpacing;
            int startY = (windowHeight - totalHeight) / 2;
            int startX = (windowWidth - buttonWidth) / 2;
            
            if (checkButton(mouseX, mouseY, startX, startY, buttonWidth, buttonHeight)) {
                gameStatus = GameStatus::PLAYING;
                levelParameters.cellSize = 48;
                start();
            }
            else if (checkButton(mouseX, mouseY, startX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight)) {
                loadGame();
            }
            else if (checkButton(mouseX, mouseY, startX, startY + 2 * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight)) {
                return false;
            }
            break;
        }
            
        case GameStatus::VICTORY: {
            int totalHeight = 2 * buttonHeight + buttonSpacing;
            int startY = (windowHeight - totalHeight) / 2 + 100;
            int startX = (windowWidth - buttonWidth) / 2;
            
            if (checkButton(mouseX, mouseY, startX, startY, buttonWidth, buttonHeight)) {
                gameStatus = GameStatus::IMPROVE;
            }
            else if (checkButton(mouseX, mouseY, startX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight)) {
                gameStatus = GameStatus::MAIN_MENU;
            }
            break;
        }

        case GameStatus::DEFEAT: {
            int startY = (windowHeight - buttonHeight) / 2 + 100;
            int startX = (windowWidth - buttonWidth) / 2;

             if (checkButton(mouseX, mouseY, startX, startY, buttonWidth, buttonHeight)) {
                gameStatus = GameStatus::MAIN_MENU;
            }
            break;
        }

        case GameStatus::IMPROVE: {
            buttonWidth = 300;
            buttonHeight = 100;
            int totalHeight = 3 * buttonHeight + 2 * buttonSpacing;
            int startY = (windowHeight - totalHeight) / 2;
            int startX = (windowWidth - buttonWidth) / 2;
            
            if (checkButton(mouseX, mouseY, startX, startY, buttonWidth, buttonHeight)) {
                levelParameters.playerImproveHP += 10;
                goToNextLVL = 1;
            }
            else if (checkButton(mouseX, mouseY, startX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight)) {
                levelParameters.playerDamageKoef += 1;
                goToNextLVL = 1;
            }
            else if (checkButton(mouseX, mouseY, startX, startY + 2 * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight)) {
                levelParameters.spellDamageKoef += 1;
                goToNextLVL = 1;
            }
            
            if (goToNextLVL == 1){
                gameStatus = GameStatus::PLAYING;
                nextLevel();
            }
            break;

        }
    }
    return true;
}

bool Game::checkButton(int mouseX, int mouseY, int btnX, int btnY, int btnWidth, int btnHeight) {
    return mouseX >= btnX && mouseX <= btnX + btnWidth &&
           mouseY >= btnY && mouseY <= btnY + btnHeight;    
    }

void Game::saveGame(){
    SaveData data = gameManager.convertToSaveData();
    data.gameCondition = static_cast<int>(gameStatus);
    data.playerDamageKoef = levelParameters.playerDamageKoef;
    data.spellsKoef = levelParameters.spellDamageKoef;
    data.goalMoves = levelParameters.goalMoves;
    data.goalScore = levelParameters.goalScore;
    data.playerImproveHP = levelParameters.playerImproveHP;
    data.cellSize = (levelParameters.enemyDamage % 2 == 0) ? 40 : 48;
    data.hash = saver.makeHash(data);
    if (!saver.saveToJson(data)){
        std::cout << "Failed to save game." << std::endl;
    }
}

void Game::loadGame(){
    SaveData data;
    if (!saver.loadFromJson(data) || !saver.checkSaveData(data)){
        std::cout << "Failed to load saved game. File is incorrect or empty." << std::endl;
    } else {
        gameStatus = static_cast<GameStatus>(data.gameCondition);

        levelParameters.spellDamageKoef = data.spellsKoef;
        levelParameters.playerDamageKoef = data.playerDamageKoef;
        levelParameters.playerHP = data.playerHealth;
        levelParameters.playerImproveHP = data.playerImproveHP;
        levelParameters.goalMoves = data.goalMoves;
        levelParameters.goalScore = data.goalScore;
        levelParameters.enemyDamage = data.enemyDamage;
        levelParameters.enemyHP = data.enemyHealth;
        levelParameters.fieldLength = data.fieldLength;
        levelParameters.fieldWidth = data.fieldWidth;

        levelParameters.cellSize = data.cellSize;
        gameManager.unpackSaveData(data);
    }
}

void Game::nextLevel(){
    Player player = gameManager.getPlayer();
    int handCurrentSize = player.getHandFullness();
    player.deleteRandomSpell(handCurrentSize / 2);

    while (!levelParameters.spells.empty()){
        levelParameters.spells.pop_back();
    }

    for (int i = 0; i < player.getHandFullness(); ++i){
        levelParameters.spells.push_back(player.getSpellName(i));
    }

    if (levelParameters.fieldLength == 16 && levelParameters.fieldWidth == 18){
        levelParameters.fieldLength = 15;
        levelParameters.fieldWidth = 15;
        levelParameters.cellSize = 48;
    } else {
        levelParameters.fieldLength = 16;
        levelParameters.fieldWidth = 18;
        levelParameters.cellSize = 40;
    }

    levelParameters.enemyDamage += 1;
    levelParameters.goalMoves += 5;
    levelParameters.goalScore += 1;
    levelParameters.playerHP = 10 + 5 * (levelParameters.enemyDamage - 1) + levelParameters.playerImproveHP;
    levelParameters.enemyHP = 5 + (levelParameters.enemyDamage - 1);

    start();
}

GameStatus Game::getGameStatus() const{
    return gameStatus;
}

LevelAttributes Game::getLVLParameters() const{
    return levelParameters;
}

GameManager Game::getGameManager() const{
    return gameManager;
}