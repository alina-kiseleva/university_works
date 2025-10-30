#include "../include/game.hpp"
#include <iostream>

Game::Game() : gameManager(), gameWon(0), gameLost(0) {
    Field tempField = gameManager.getField();
    int length = tempField.getLength();
    int width = tempField.getWidth();
    
    int windowWidth = width * CELL_SIZE + 200;
    int windowHeight = length * CELL_SIZE + 120;
    
    window.create(sf::VideoMode(windowWidth, windowHeight), "Cat & mice");
    
    loadAssets();

    CARDS_Y_POSITION = length * CELL_SIZE + 20;
    initializeSpellCards();
    
    infoText.setFont(font);
    infoText.setCharacterSize(14);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition(width * CELL_SIZE + 10, 10);

    instructionText.setFont(font);
    instructionText.setCharacterSize(14);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(width * CELL_SIZE + 10, 310);
    instructionText.setString(
        "CONTROLS:\n"
        "WASD - Move\n"
        "Q - Far Attack (Range: 2)\n" 
        "E - Close Attack (Range: 1)\n"
        "F - Attack\n"
        "R - Buy spell\n"
        "1-3 - Select spell\n"
        "Esc - Cancel spell\n"
        "Click - Select target\n\n"
        "GOAL:\n"
        "Defeat 4 enemies\n"
        "in under 60 moves!"
    );
    
    victoryText.setFont(fontResult);
    victoryText.setCharacterSize(48);
    victoryText.setFillColor(sf::Color::Green);
    victoryText.setString("VICTORY!");
    victoryText.setStyle(sf::Text::Bold);
    
    gameOverText.setFont(fontResult);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setStyle(sf::Text::Bold);
}

void Game::checkGameState() {
    Player player = gameManager.getPlayer();
    int moves = gameManager.getMoves();
    int score = player.getScore();
    bool current_condition = gameManager.getIsFinished();
    
    if (score >= 4 && moves <= 60) {
        gameWon = true;
    } else if (moves > 60 || !player.isAlive()) {
        gameLost = true;
    }

    if (gameLost || gameWon){
        gameManager.setIsFinished(current_condition);
    }
}

void Game::loadAssets() {
    playerTexture.loadFromFile("../assets/textures/tom.png");
    enemyTexture.loadFromFile("../assets/textures/agro_jerry_empty.png");
    defaultCellTexture.loadFromFile("../assets/textures/grass.png");
    obstacleCellTexture.loadFromFile("../assets/textures/obst.png");
    freezeCellTexture.loadFromFile("../assets/textures/freezer.png");
    trapCellTexture.loadFromFile("../assets/textures/trap.png");
    towerTexture.loadFromFile("../assets/textures/tower.png");
    font.loadFromFile("../assets/fonts/Battambang-Light.ttf");
    fontResult.loadFromFile("../assets/fonts/Battambang-Bold.ttf");
}

void Game::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();

        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left && !gameWon && !gameLost) {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
       
        } else if (event.type == sf::Event::KeyPressed) {
            if (gameWon || gameLost) {continue;} 

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
                    playerActed = 1;
                    break;
                case sf::Keyboard::R:
                    gameManager.buySpell();
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
                EnemyTower tower = gameManager.getTower();
                auto [towerX, towerY] = tower.getCoordinates();
                auto [playerX, playerY] = player.getCoordinates();
                int distance = std::abs(towerX - playerX) + std::abs(towerY - playerY);
                
                if (distance <= 3) { // Радиус атаки башни
                    showRangeOverlay = true;
                    currentOverlayType = OverlayType::TOWER_RANGE;
                }

                if (isTowerAttack && timeWithoutAttack > 0){
                    --timeWithoutAttack;
                } else {
                    isTowerAttack = gameManager.getTower().attackTarget(player);
                    gameManager.setPlayer(player);
                    timeWithoutAttack = 2;
                }
            }
            checkGameState();
        }
    }
}

void Game::handleMouseClick(int mouseX, int mouseY){
    Field field = gameManager.getField();
    int length = field.getLength();
    int width = field.getWidth();
    
    if (mouseX < width * CELL_SIZE){
        if (waitingForTarget && selectedSpellCard != -1){
            int cellX = mouseY / CELL_SIZE;
            int cellY = mouseX / CELL_SIZE;
            
            if (cellX >= 0 && cellX < length && 
                cellY >= 0 && cellY < width){
                
                bool success = gameManager.applySpell(selectedSpellCard, {cellX, cellY});
                
                if (success){
                    gameManager.moveEnemy();
                    checkGameState();
                }
                
                selectedSpellCard = -1;
                waitingForTarget = false;
            }
        }
    }
}

void Game::initializeSpellCards() {
    cardBackgrounds.clear();
    cardTitles.clear();
    cardDescriptions.clear();
    
    Player player = gameManager.getPlayer();
    int cardCount = player.getHandSize();
    
    // Вычисляем начальную позицию для центрирования
    Field field = gameManager.getField();
    int totalWidth = field.getWidth() * CELL_SIZE;
    int cardsTotalWidth = cardCount * CARD_WIDTH + (cardCount - 1) * CARD_MARGIN;
    int startX = (totalWidth - cardsTotalWidth) / 2;
    
    for (int i = 0; i < cardCount; ++i) {
        // Фон карточки
        sf::RectangleShape cardBackground(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT));
        cardBackground.setPosition(startX + i * (CARD_WIDTH + CARD_MARGIN), CARDS_Y_POSITION);
        cardBackground.setFillColor(sf::Color(50, 50, 80)); // Темно-синий фон
        cardBackground.setOutlineThickness(2);
        cardBackground.setOutlineColor(sf::Color::White);
        cardBackgrounds.push_back(cardBackground);
        
        // Заголовок карточки
        sf::Text cardTitle;
        cardTitle.setFont(font);
        cardTitle.setCharacterSize(12);
        cardTitle.setFillColor(sf::Color::Yellow);
        cardTitle.setPosition(startX + i * (CARD_WIDTH + CARD_MARGIN) + 5, CARDS_Y_POSITION + 5);
        
        // Описание карточки
        sf::Text cardDescription;
        cardDescription.setFont(font);
        cardDescription.setCharacterSize(10);
        cardDescription.setFillColor(sf::Color::White);
        cardDescription.setPosition(startX + i * (CARD_WIDTH + CARD_MARGIN) + 5, CARDS_Y_POSITION + 25);
        
        // Получаем информацию о заклинании
        if (player.hasSpell(i)) {
            spellType type = player.getSpellName(i);
            std::string title, description;
            
            switch (type) {
                case spellType::DIRECT_DAMAGE_SPELL:
                    title = "Direct Damage";
                    description = "Single target\nRange: 4\nDamage: 2";
                    cardBackground.setFillColor(sf::Color(120, 50, 50)); // Красный
                    break;
                case spellType::AREA_DAMAGE_SPELL:
                    title = "Area Damage";
                    description = "2x2 area\nRange: 5\nDamage: 1";
                    cardBackground.setFillColor(sf::Color(50, 120, 50)); // Зеленый
                    break;
                case spellType::TRAP_SPELL:
                    title = "Trap";
                    description = "Place trap\nEnemy takes\ndamage on step";
                    cardBackground.setFillColor(sf::Color(50, 50, 120)); // Синий
                    break;
            }
            
            cardTitle.setString(title);
            cardDescription.setString(description);
        } else {
            cardTitle.setString("Empty Slot");
            cardDescription.setString("No spell\nPress R to buy");
            cardBackground.setFillColor(sf::Color(80, 80, 80)); // Серый
        }
        
        cardTitles.push_back(cardTitle);
        cardDescriptions.push_back(cardDescription);
    }
}

void Game::update() {
    Player player = gameManager.getPlayer();
    Enemy enemy = gameManager.getEnemy();
    int score = player.getScore();
    int moves = gameManager.getMoves();

    updateRangeOverlay();
    
    // Обновляем внешний вид карточек
    for (int i = 0; i < cardBackgrounds.size(); ++i) {
        // Обновляем цвет рамки для выбранной карточки
        if (i == selectedSpellCard) {
            cardBackgrounds[i].setOutlineColor(sf::Color::Yellow);
            cardBackgrounds[i].setOutlineThickness(4);
        } else {
            cardBackgrounds[i].setOutlineColor(sf::Color::White);
            cardBackgrounds[i].setOutlineThickness(2);
        }
        
        // Обновляем содержимое карточек
        if (player.hasSpell(i)) {
            spellType type = player.getSpellName(i);
            std::string title, description;
            
            switch (type) {
                case spellType::DIRECT_DAMAGE_SPELL:
                    title = "Direct Damage";
                    description = "Single target\nRange: 3\nDamage: 2";
                    cardBackgrounds[i].setFillColor(sf::Color(120, 50, 50));
                    break;
                case spellType::AREA_DAMAGE_SPELL:
                    title = "Area Damage";
                    description = "2x2 area\nRange: 4\nDamage: 1";
                    cardBackgrounds[i].setFillColor(sf::Color(50, 120, 50));
                    break;
                case spellType::TRAP_SPELL:
                    title = "Trap";
                    description = "Place trap\nEnemy takes\ndamage on step";
                    cardBackgrounds[i].setFillColor(sf::Color(50, 50, 120));
                    break;
            }
            
            cardTitles[i].setString(title);
            cardDescriptions[i].setString(description);
        } else {
            cardTitles[i].setString("Empty Slot");
            cardDescriptions[i].setString("No spell\nPress R to buy");
            cardBackgrounds[i].setFillColor(sf::Color(80, 80, 80));
        }
    }

    infoText.setString(
        std::string("PLAYER\n") + 
        "Health: " + std::to_string(player.getHealth()) + "\n" +
        "Score: " + std::to_string(score) + "/4\n" +
        "Coins: " + std::to_string(player.getCoins()) + "\n" +
        "Moves: " + std::to_string(moves) + "/60\n" +
        "Damage: " + (player.getDamage() == typeOfFight::CLOSE ? "Close(2)" : "Far(1)") + "\n" +
        "Can move: " + (player.getMoveAbility() ? "YES" : "NO") + "\n\n" + 
        "ENEMY\n" +
        "Health: " + std::to_string(enemy.getHealth()) + "\n" + 
        "Damage: " + std::to_string(enemy.getDamage())
    );
    
    // Центрируем тексты победы/проигрыша
    sf::FloatRect victoryBounds = victoryText.getLocalBounds();
    victoryText.setPosition(
        (window.getSize().x - victoryBounds.width) / 2,
        (window.getSize().y - victoryBounds.height) / 2
    );
    
    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(
        (window.getSize().x - gameOverBounds.width) / 2,
        (window.getSize().y - gameOverBounds.height) / 2
    );
}

void Game::render() {
    window.clear(sf::Color::Black);

    Field field = gameManager.getField();
    int length = field.getLength();
    int width = field.getWidth();

    // Отрисовываем поле
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j) {
            typeOfCell cellType = field.getCellType(i, j);
            character cellChar = field.getCellCharacter(i, j);
            
            sf::Sprite cellSprite;
            switch (cellType) {
                case typeOfCell::DEFAULT:
                    cellSprite.setTexture(defaultCellTexture);
                    cellSprite.setTextureRect(sf::IntRect(30, 30, 48, 48));
                    break;
                case typeOfCell::OBSTACLE:
                    cellSprite.setTexture(obstacleCellTexture);
                    break;
                case typeOfCell::FREEZE:
                    cellSprite.setTexture(freezeCellTexture);
                    cellSprite.setTextureRect(sf::IntRect(30, 30, 48, 48));
                    break;
            }
            // Устанавливаем квадратный размер
            cellSprite.setPosition(j * CELL_SIZE, i * CELL_SIZE);
            cellSprite.setScale(
                CELL_SIZE / cellSprite.getLocalBounds().width,
                CELL_SIZE / cellSprite.getLocalBounds().height
            );
            window.draw(cellSprite);

            if (cellChar == character::PLAYER) {
                sf::Sprite playerSprite;
                playerSprite.setTexture(playerTexture);
                playerSprite.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                playerSprite.setScale(
                    CELL_SIZE / playerSprite.getLocalBounds().width,
                    CELL_SIZE / playerSprite.getLocalBounds().height
                );
                window.draw(playerSprite);
            }
            else if (cellChar == character::ENEMY) {
                sf::Sprite enemySprite;
                enemySprite.setTexture(enemyTexture);
                enemySprite.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                enemySprite.setScale(
                    CELL_SIZE / enemySprite.getLocalBounds().width,
                    CELL_SIZE / enemySprite.getLocalBounds().height
                );
                window.draw(enemySprite);
            } else if (cellChar == character::ENEMY_TOWER){
                sf::Sprite towerSprite;
                towerSprite.setTexture(towerTexture);
                towerSprite.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                towerSprite.setScale(
                    CELL_SIZE / towerSprite.getLocalBounds().width,
                    CELL_SIZE / towerSprite.getLocalBounds().height
                );
                window.draw(towerSprite);
            } else if (cellChar == character::TRAP){
                sf::Sprite trapSprite;
                trapSprite.setTexture(trapCellTexture);
                trapSprite.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                trapSprite.setScale(
                    CELL_SIZE / trapSprite.getLocalBounds().width,
                    CELL_SIZE / trapSprite.getLocalBounds().height
                );
                window.draw(trapSprite);
            }
        }
    }

    for (const auto& overlay : rangeOverlay) {
        window.draw(overlay);
    }

     // Отрисовка фона для бара карточек
    sf::RectangleShape cardsBackground;
    cardsBackground.setSize(sf::Vector2f(window.getSize().x, 120));
    cardsBackground.setPosition(0, field.getLength() * CELL_SIZE);
    cardsBackground.setFillColor(sf::Color(40, 40, 40));
    window.draw(cardsBackground);

    // Отрисовка карточек заклинаний
    for (auto& card : cardBackgrounds) {
        window.draw(card);
    }
    for (auto& title : cardTitles) {
        window.draw(title);
    }
    for (auto& description : cardDescriptions) {
        window.draw(description);
    }

    // Рисуем темный фон для UI панели
    int fieldWidth = width * CELL_SIZE;
    sf::RectangleShape uiBackground;
    uiBackground.setSize(sf::Vector2f(window.getSize().x - fieldWidth, window.getSize().y));
    uiBackground.setPosition(fieldWidth, 0);
    uiBackground.setFillColor(sf::Color(40, 40, 40));
    window.draw(uiBackground);

    window.draw(infoText);
    window.draw(instructionText);

    if (gameWon) {
        // Затемняем экран
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Полупрозрачный черный
        window.draw(overlay);
        
        window.draw(victoryText);
    } else if (gameLost) {
        // Затемняем экран
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        window.draw(gameOverText);
        
        Player player = gameManager.getPlayer();
        sf::Text reasonText;
        reasonText.setFont(font);
        reasonText.setCharacterSize(24);
        reasonText.setFillColor(sf::Color::White);
        
        std::string reason = "Out of moves!";
        if (!player.isAlive()) reason = "Player died!";
        
        reasonText.setString(reason);
        reasonText.setPosition(
            (window.getSize().x - reasonText.getLocalBounds().width) / 2,
            gameOverText.getPosition().y + gameOverText.getLocalBounds().height + 10
        );
        window.draw(reasonText);
    }

     window.display();
}

void Game::updateRangeOverlay() {
    clearRangeOverlay();
    
    if (!showRangeOverlay) return;
    
    Player player = gameManager.getPlayer();
    auto [playerX, playerY] = player.getCoordinates();
    Field field = gameManager.getField();
    
    sf::Color overlayColor;
    
    switch (currentOverlayType) {
        case OverlayType::SPELL_RANGE:
            if (selectedSpellCard != -1 && player.hasSpell(selectedSpellCard)) {
                spellType type = player.getSpellName(selectedSpellCard);
                int range = 0;
                if (type == spellType::DIRECT_DAMAGE_SPELL) range = 3;
                else if (type == spellType::AREA_DAMAGE_SPELL) range = 4;
                if (range > 0){
                    showSpellRange(range);
                }
            }
            break;
            
        case OverlayType::TOWER_RANGE:
            showTowerRange();
            break;
            
        case OverlayType::COMBAT_RANGE:
            showCombatRange();
            break;
            
        case OverlayType::NONE:
            break;
    }
}

void Game::showSpellRange(int range) {
    Player player = gameManager.getPlayer();
    auto [playerX, playerY] = player.getCoordinates();
    Field field = gameManager.getField();
    
    for (int i = 0; i < field.getLength(); ++i) {
        for (int j = 0; j < field.getWidth(); ++j) {
            int distance = std::abs(playerX - i) + std::abs(playerY - j);
            if (distance <= range && field.canMoveTo(i, j)) {
                sf::RectangleShape overlay(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                overlay.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                overlay.setFillColor(sf::Color(128, 0, 128, 100)); // Полупрозрачный фиолетовый
                rangeOverlay.push_back(overlay);
            }
        }
    }
}

void Game::showTowerRange() {
    EnemyTower tower = gameManager.getTower();
    auto [towerX, towerY] = tower.getCoordinates();
    Field field = gameManager.getField();
    int towerRange = 3; // Радиус атаки башни
    
    for (int i = 0; i < field.getLength(); ++i) {
        for (int j = 0; j < field.getWidth(); ++j) {
            int distance = std::abs(towerX - i) + std::abs(towerY - j);
            if (distance <= towerRange) {
                sf::RectangleShape overlay(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                overlay.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                overlay.setFillColor(sf::Color(255, 0, 0, 80)); // Полупрозрачный красный
                rangeOverlay.push_back(overlay);
            }
        }
    }
}

void Game::showCombatRange() {
    Player player = gameManager.getPlayer();
    auto [playerX, playerY] = player.getCoordinates();
    Field field = gameManager.getField();
    int range = (player.getDamage() == typeOfFight::CLOSE) ? 1 : 2;
    
    for (int i = 0; i < field.getLength(); ++i) {
        for (int j = 0; j < field.getWidth(); ++j) {
            int distance = std::abs(playerX - i) + std::abs(playerY - j);
            if (distance <= range) {
                sf::RectangleShape overlay(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                overlay.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                overlay.setFillColor(sf::Color(0, 0, 255, 80)); // Полупрозрачный синий
                rangeOverlay.push_back(overlay);
            }
        }
    }
}

void Game::clearRangeOverlay() {
    rangeOverlay.clear();
}

void Game::run() {
    gameManager.placeCharacter(character::ENEMY);
    gameManager.placeCharacter(character::ENEMY_TOWER);
    while (window.isOpen()) {
        handleInput();
        update();
        render();

        sf::sleep(sf::milliseconds(16));
    }
}