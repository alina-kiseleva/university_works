#include "../include/game.hpp"
#include <iostream>

Game::Game() : gameStatus(GameStatus::MAIN_MENU), gameManager(GameManager()), 
            GUI(gameManager.getField().getLength(), gameManager.getField().getWidth()){

    Field field = gameManager.getField();
    int length = field.getLength();
    int width = field.getWidth();

    windowWidth = width * CELL_SIZE + 200;
    windowHeight = length * CELL_SIZE + 120;

    window.create(sf::VideoMode(windowWidth, windowHeight), "Cat & mice");
}

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
    GUI.updateCellSize(CELL_SIZE);
    GUI.initializeSpellCards(gameManager.getPlayer(), gameManager.getField());

    while (window.isOpen()){
        handleInput();
        switch (gameStatus) {
            case GameStatus::MAIN_MENU:
                GUI.renderMainMenu(window, gameStatus);
                break;
            case GameStatus::IMPROVE:
                GUI.renderImprove(window, gameManager, levelParameters.playerHP);
                break;
            case GameStatus::PLAYING:
                GUI.updateCellSize(CELL_SIZE);
                GUI.update(gameManager, selectedSpellCard, window, currentOverlayType, showRangeOverlay, levelParameters);
                GUI.render(gameManager, window, gameStatus);
                break;
            case GameStatus::VICTORY:
                GUI.renderLevelComplete(window, gameManager, levelParameters.goalScore);
                break;
            case GameStatus::DEFEAT:
                GUI.renderGameOver(window, gameManager, levelParameters);
                break;
        }
        sf::sleep(sf::milliseconds(16));
    }
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

void Game::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();

        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left){
                if (gameStatus == GameStatus::PLAYING){
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                } else {
                    handleMenuClick(event.mouseButton.x, event.mouseButton.y);
                }
            }

        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::L){
                saveGame();
            }

            if (gameStatus != GameStatus::PLAYING) {continue;} 

            auto player = gameManager.getPlayer();
            int movesBefore = gameManager.getMoves();
            bool playerActed = 0;

            switch (event.key.code) {
                case sf::Keyboard::W:
                case sf::Keyboard::A:
                case sf::Keyboard::S:
                case sf::Keyboard::D:
                    gameManager.movePlayer(
                        event.key.code == sf::Keyboard::W ? 'w' :
                        event.key.code == sf::Keyboard::A ? 'a' :
                        event.key.code == sf::Keyboard::S ? 's' : 'd'
                    );
                    if (movesBefore < gameManager.getMoves()){
                        playerActed = 1;
                    }
                    break;
                case sf::Keyboard::Q:
                    playerActed = player.selectCombatMode(typeOfFight::FAR);
                    if (playerActed){
                        gameManager.setPlayer(player);
                        gameManager.setMoves(gameManager.getMoves() + 1);
                        showRangeOverlay = true;
                        currentOverlayType = OverlayType::COMBAT_RANGE;
                    }
                    break;
                case sf::Keyboard::E:
                    playerActed = player.selectCombatMode(typeOfFight::CLOSE);
                    if (playerActed){
                        gameManager.setPlayer(player);
                        gameManager.setMoves(gameManager.getMoves() + 1);
                        showRangeOverlay = true;
                        currentOverlayType = OverlayType::COMBAT_RANGE;
                    }
                    break;
                case sf::Keyboard::F:
                    gameManager.attackEnemy();
                    gameManager.setMoves(gameManager.getMoves() + 1);
                    playerActed = 1;
                    break;
                case sf::Keyboard::R:
                    gameManager.buySpell(levelParameters.spellDamageKoef);
                    if (movesBefore < gameManager.getMoves()){
                        playerActed = 1;
                    }
                    break;
                case sf::Keyboard::Num1:
                    if (player.hasSpell(0)){
                        selectedSpellCard = 0;
                        waitingForTarget = true;
                        showRangeOverlay = true;
                        currentOverlayType = OverlayType::SPELL_RANGE;
                    }
                    break;    
                case sf::Keyboard::Num2:
                    if (player.hasSpell(1)){
                        selectedSpellCard = 1;
                        waitingForTarget = true;
                        showRangeOverlay = true;
                        currentOverlayType = OverlayType::SPELL_RANGE;
                    }
                    break;   
                case sf::Keyboard::Num3:
                    if (player.hasSpell(2)){
                        selectedSpellCard = 2;
                        waitingForTarget = true;
                        showRangeOverlay = true;
                        currentOverlayType = OverlayType::SPELL_RANGE;
                    }
                    break;
                case sf::Keyboard::Num4:
                    if (player.hasSpell(3)){
                        selectedSpellCard = 3;
                        waitingForTarget = true;
                        showRangeOverlay = true;
                        currentOverlayType = OverlayType::SPELL_RANGE;
                    }
                    break;
                case sf::Keyboard::Escape:
                    selectedSpellCard = -1;
                    waitingForTarget = false;
                    showRangeOverlay = false;
                    currentOverlayType = OverlayType::NONE;
                    break;
                default:
                    break;
            }
            if (playerActed){

                gameManager.moveEnemy();
                if (!player.getMoveAbility()){
                    player.setMoveAbility(1);
                    gameManager.setPlayer(player);
                }

                player = gameManager.getPlayer();
                auto [towerX, towerY] = gameManager.getTowerCoords();
                auto [playerX, playerY] = player.getCoordinates();
                int distance = std::abs(towerX - playerX) + std::abs(towerY - playerY);

                if (distance <= 3) { // Радиус атаки башни
                    showRangeOverlay = true;
                    currentOverlayType = OverlayType::TOWER_RANGE;
                }

                if (isTowerAttack && timeWithoutAttack > 0){
                    --timeWithoutAttack;
                } else {
                    isTowerAttack = gameManager.towerAttack(player);
                    gameManager.setPlayer(player);
                    timeWithoutAttack = 2;
                }
            }
            checkGameState();
        }
    }
}

void Game::handleMouseClick(int mouseX, int mouseY){
    if (gameStatus != GameStatus::PLAYING || !waitingForTarget || selectedSpellCard == -1) return;
    
    Field field = gameManager.getField();
    int length = field.getLength();
    int width = field.getWidth();
    
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
            selectedSpellCard = -1;
            waitingForTarget = false;
        }
    }
}

void Game::handleMenuClick(int mouseX, int mouseY){
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
                start();
            }
            else if (checkButton(mouseX, mouseY, startX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight)) {
                loadGame();
            }
            else if (checkButton(mouseX, mouseY, startX, startY + 2 * (buttonHeight + buttonSpacing), buttonWidth, buttonHeight)) {
                window.close();
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
    data.cellSize = CELL_SIZE;
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

        CELL_SIZE = data.cellSize;
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
        CELL_SIZE = 48;
    } else {
        levelParameters.fieldLength = 16;
        levelParameters.fieldWidth = 18;
        CELL_SIZE = 40;
    }

    levelParameters.enemyDamage += 1;
    levelParameters.goalMoves += 5;
    levelParameters.goalScore += 1;
    levelParameters.playerHP = 10 + 5 * (levelParameters.enemyDamage - 1) + levelParameters.playerImproveHP;
    levelParameters.enemyHP = 5 + (levelParameters.enemyDamage - 1);

    start();
}