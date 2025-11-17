#include "../include/game.hpp"
#include <iostream>

GameGraphics::GameGraphics(int length, int width){
    loadAssets();
    initializeMenuUI();
    initializeImproveUI();

    CARDS_Y_POSITION = length * CELL_SIZE + 20;

    infoText.setFont(font);
    infoText.setCharacterSize(14);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition(width * CELL_SIZE + 10, 10);

    instructionText.setFont(font);
    instructionText.setCharacterSize(14);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(width * CELL_SIZE + 10, 310);

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

void GameGraphics::loadAssets() {
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

void GameGraphics::updateCellSize(int newSize){
    CELL_SIZE = newSize;
}

void GameGraphics::initializeSpellCards(Player player, Field field) {
    cardBackgrounds.clear();
    cardTitles.clear();
    cardDescriptions.clear();

    int cardCount = player.getHandSize();

    // Вычисляем начальную позицию для центрирования
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

void GameGraphics::update(GameManager gameManager, int selectedSpellCard, sf::RenderWindow& window, 
    OverlayType currentOverlayType, bool showRangeOverlay, LevelAttributes lvl) {
   
    Player player = gameManager.getPlayer();
    Enemy enemy = gameManager.getEnemy();
    int score = player.getScore();
    int moves = gameManager.getMoves();

    updateRangeOverlay(gameManager, currentOverlayType, showRangeOverlay, selectedSpellCard);

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
                    description = "Single target\nRange: 3\nDamage: " + std::to_string(2*lvl.spellDamageKoef);
                    cardBackgrounds[i].setFillColor(sf::Color(120, 50, 50));
                    break;
                case spellType::AREA_DAMAGE_SPELL:
                    title = "Area Damage";
                    description = "2x2 area\nRange: 4\nDamage: " + std::to_string(lvl.spellDamageKoef);
                    cardBackgrounds[i].setFillColor(sf::Color(50, 120, 50));
                    break;
                case spellType::TRAP_SPELL:
                    title = "Trap";
                    description = "Place trap\nEnemy takes\ndamage " + std::to_string(lvl.spellDamageKoef) + " on step";
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
        "Score: " + std::to_string(score) + "/" + std::to_string(lvl.goalScore) + "\n" +
        "Coins: " + std::to_string(player.getCoins()) + "\n" +
        "Moves: " + std::to_string(moves) + "/" + std::to_string(lvl.goalMoves) + "\n" +
        "Damage: " + (player.getCombatType() == typeOfFight::CLOSE ? "Close(" + std::to_string(2*lvl.playerDamageKoef) + ")" : 
        "Far(" + std::to_string(lvl.playerDamageKoef) + ")" ) + "\n" +
        "Can move: " + (player.getMoveAbility() ? "YES" : "NO") + "\n\n" + 
        "ENEMY\n" +
        "Health: " + std::to_string(enemy.getHealth()) + "\n" + 
        "Damage: " + std::to_string(enemy.getDamage())
    );

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
        "Defeat " + std::to_string(lvl.goalScore) + " enemies\n"
        "in under " + std::to_string(lvl.goalMoves) + " moves!"
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

void GameGraphics::render(GameManager gameManager, sf::RenderWindow& window, GameStatus gameStatus) {
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
                    cellSprite.setTextureRect(sf::IntRect(30, 30, CELL_SIZE, CELL_SIZE));
                    break;
                case typeOfCell::OBSTACLE:
                    cellSprite.setTexture(obstacleCellTexture);
                    break;
                case typeOfCell::FREEZE:
                    cellSprite.setTexture(freezeCellTexture);
                    cellSprite.setTextureRect(sf::IntRect(30, 30, CELL_SIZE, CELL_SIZE));
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
    cardsBackground.setPosition(0, field.getWidth() * CELL_SIZE);
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

    window.display();
}

void GameGraphics::updateRangeOverlay(GameManager gameManager, OverlayType currentOverlayType, bool showRangeOverlay, int selectedSpellCard) {
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
                    showSpellRange(gameManager, range);
                }
            }
            break;

        case OverlayType::TOWER_RANGE:
            showTowerRange(gameManager);
            break;

        case OverlayType::COMBAT_RANGE:
            showCombatRange(gameManager);
            break;

        case OverlayType::NONE:
            break;
    }
}

void GameGraphics::showSpellRange(GameManager gameManager, int range) {
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

void GameGraphics::showTowerRange(GameManager gameManager) {
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

void GameGraphics::showCombatRange(GameManager gameManager) {
    Player player = gameManager.getPlayer();
    auto [playerX, playerY] = player.getCoordinates();
    Field field = gameManager.getField();
    int range = (player.getCombatType() == typeOfFight::CLOSE) ? 1 : 2;

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

void GameGraphics::clearRangeOverlay() {
    rangeOverlay.clear();
}

void GameGraphics::initializeMenuUI() {
    // Заголовок главного меню
    menuTitle.setFont(fontResult);
    menuTitle.setString("CAT & MICE");
    menuTitle.setCharacterSize(64);
    menuTitle.setFillColor(sf::Color::Yellow);
    menuTitle.setStyle(sf::Text::Bold);

    // Заголовок завершения уровня
    levelCompleteTitle.setFont(fontResult);
    levelCompleteTitle.setString("LEVEL COMPLETED!");
    levelCompleteTitle.setCharacterSize(48);
    levelCompleteTitle.setFillColor(sf::Color::Green);
    levelCompleteTitle.setStyle(sf::Text::Bold);
}

void GameGraphics::renderMainMenu(sf::RenderWindow& window, GameStatus gameStatus) {
    window.clear(sf::Color(30, 30, 50)); // Темно-синий фон

    // Центрируем и рисуем заголовок
    sf::FloatRect titleBounds = menuTitle.getLocalBounds();
    menuTitle.setPosition(
        (window.getSize().x - titleBounds.width) / 2,
        100
    );
    window.draw(menuTitle);

    // Создаем кнопки главного меню
    menuButtons.clear();
    menuButtonTexts.clear();

    std::vector<std::string> buttonLabels = {"New Game", "Continue", "Exit"};
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonSpacing = 20;
    int totalHeight = buttonLabels.size() * buttonHeight + (buttonLabels.size() - 1) * buttonSpacing;
    int startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < buttonLabels.size(); ++i) {
        // Фон кнопки
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition((window.getSize().x - buttonWidth) / 2, 
                          startY + i * (buttonHeight + buttonSpacing));
        button.setFillColor(sf::Color(70, 70, 100));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::White);
        menuButtons.push_back(button);

        // Текст кнопки
        sf::Text buttonText;
        buttonText.setFont(font);
        buttonText.setString(buttonLabels[i]);
        buttonText.setCharacterSize(24);
        buttonText.setFillColor(sf::Color::White);
        
        sf::FloatRect textBounds = buttonText.getLocalBounds();
        buttonText.setPosition(
            button.getPosition().x + (buttonWidth - textBounds.width) / 2,
            button.getPosition().y + (buttonHeight - textBounds.height) / 2 - 5
        );
        menuButtonTexts.push_back(buttonText);
    }

    // Рисуем кнопки
    for (const auto& button : menuButtons) {
        window.draw(button);
    }
    for (const auto& text : menuButtonTexts) {
        window.draw(text);
    }

    // Информация о сохранении
    sf::Text saveInfo;
    saveInfo.setFont(font);
    saveInfo.setString("Press 'L' to save progress");
    saveInfo.setCharacterSize(16);
    saveInfo.setFillColor(sf::Color::White);
    saveInfo.setPosition(
        (window.getSize().x - saveInfo.getLocalBounds().width) / 2,
        window.getSize().y - 50
    );
    window.draw(saveInfo);

    window.display();
}

void GameGraphics::renderLevelComplete(sf::RenderWindow& window, const GameManager& gameManager, int goalScore) {
    window.clear(sf::Color(30, 50, 30)); // Темно-зеленый фон

    Player player = gameManager.getPlayer();

    // Заголовок
    sf::FloatRect titleBounds = levelCompleteTitle.getLocalBounds();
    levelCompleteTitle.setPosition(
        (window.getSize().x - titleBounds.width) / 2,
        150
    );
    window.draw(levelCompleteTitle);

    // Статистика
    sf::Text statsText;
    statsText.setFont(font);
    statsText.setString(
        "Score: " + std::to_string(player.getScore()) + "/" + std::to_string(goalScore) + "\n" +
        "Moves: " + std::to_string(gameManager.getMoves()) + "\n" +
        "Coins: " + std::to_string(player.getCoins()) + "\n" +
        "Health: " + std::to_string(player.getHealth())
    );
    statsText.setCharacterSize(24);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(
        (window.getSize().x - statsText.getLocalBounds().width) / 2,
        250
    );
    window.draw(statsText);

    // Кнопки завершения уровня
    levelCompleteButtons.clear();
    levelCompleteButtonTexts.clear();

    std::vector<std::string> buttonLabels = {"Next Level", "Menu"};
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonSpacing = 20;
    int totalHeight = buttonLabels.size() * buttonHeight + (buttonLabels.size() - 1) * buttonSpacing;
    int startY = (window.getSize().y - totalHeight) / 2 + 100;

    for (size_t i = 0; i < buttonLabels.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition((window.getSize().x - buttonWidth) / 2, 
                          startY + i * (buttonHeight + buttonSpacing));
        button.setFillColor(sf::Color(70, 100, 70));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::White);
        levelCompleteButtons.push_back(button);

        sf::Text buttonText;
        buttonText.setFont(font);
        buttonText.setString(buttonLabels[i]);
        buttonText.setCharacterSize(24);
        buttonText.setFillColor(sf::Color::White);
        
        sf::FloatRect textBounds = buttonText.getLocalBounds();
        buttonText.setPosition(
            button.getPosition().x + (buttonWidth - textBounds.width) / 2,
            button.getPosition().y + (buttonHeight - textBounds.height) / 2 - 5
        );
        levelCompleteButtonTexts.push_back(buttonText);
    }

    // Рисуем кнопки
    for (const auto& button : levelCompleteButtons) {
        window.draw(button);
    }
    for (const auto& text : levelCompleteButtonTexts) {
        window.draw(text);
    }

    sf::Text saveInfo;
    saveInfo.setFont(font);
    saveInfo.setString("Press 'L' to save progress");
    saveInfo.setCharacterSize(16);
    saveInfo.setFillColor(sf::Color::White);
    saveInfo.setPosition(
        (window.getSize().x - saveInfo.getLocalBounds().width) / 2,
        window.getSize().y - 50
    );
    window.draw(saveInfo);

    window.display();
}

void GameGraphics::renderGameOver(sf::RenderWindow& window, const GameManager& gameManager, LevelAttributes lvl) {
    window.clear(sf::Color(50, 30, 30)); // Темно-красный фон

    Player player = gameManager.getPlayer();

    // Заголовок
    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(
        (window.getSize().x - gameOverBounds.width) / 2,
        150
    );
    window.draw(gameOverText);

    // Причина проигрыша
    sf::Text reasonText;
    reasonText.setFont(font);
    reasonText.setCharacterSize(24);
    reasonText.setFillColor(sf::Color::White);

    std::string reason = "Out of moves!";
    if (!player.isAlive()) reason = "Player died!";
    reasonText.setString(reason);

    reasonText.setPosition(
        (window.getSize().x - reasonText.getLocalBounds().width) / 2,
        250
    );
    window.draw(reasonText);

    // Статистика
    sf::Text statsText;
    statsText.setFont(font);
    statsText.setString(
        "Score: " + std::to_string(player.getScore()) + "/" + std::to_string(lvl.goalScore) + "\n" +
        "Moves: " + std::to_string(gameManager.getMoves()) + "/" + std::to_string(lvl.goalMoves) + "\n" +
        "Coins: " + std::to_string(player.getCoins())
    );
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(
        (window.getSize().x - statsText.getLocalBounds().width) / 2,
        300
    );
    window.draw(statsText);

    // Кнопка возврата в меню
    gameOverButtons.clear();
    gameOverButtonTexts.clear();

    int buttonWidth = 200;
    int buttonHeight = 50;
    int startY = (window.getSize().y - buttonHeight) / 2 + 100;

    sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
    button.setPosition((window.getSize().x - buttonWidth) / 2, startY);
    button.setFillColor(sf::Color(100, 70, 70));
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::White);
    gameOverButtons.push_back(button);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("Menu");
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        button.getPosition().x + (buttonWidth - textBounds.width) / 2,
        button.getPosition().y + (buttonHeight - textBounds.height) / 2 - 5
    );
    gameOverButtonTexts.push_back(buttonText);

    // Рисуем кнопку
    window.draw(button);
    window.draw(buttonText);

    sf::Text saveInfo;
    saveInfo.setFont(font);
    saveInfo.setString("Press 'L' to save progress");
    saveInfo.setCharacterSize(16);
    saveInfo.setFillColor(sf::Color::White);
    saveInfo.setPosition(
        (window.getSize().x - saveInfo.getLocalBounds().width) / 2,
        window.getSize().y - 50
    );
    window.draw(saveInfo);

    window.display();
}

void GameGraphics::initializeImproveUI() {
    // Заголовок экрана улучшений
    improveTitle.setFont(fontResult);
    improveTitle.setString("CHOOSE IMPROVEMENT");
    improveTitle.setCharacterSize(48);
    improveTitle.setFillColor(sf::Color::Yellow);
    improveTitle.setStyle(sf::Text::Bold);
}

void GameGraphics::renderImprove(sf::RenderWindow& window, const GameManager& gameManager, int playerHP) {
    window.clear(sf::Color(30, 30, 50)); // Темно-синий фон

    Player player = gameManager.getPlayer();

    // Заголовок
    sf::FloatRect titleBounds = improveTitle.getLocalBounds();
    improveTitle.setPosition(
        (window.getSize().x - titleBounds.width) / 2,
        100
    );
    window.draw(improveTitle);

    // Создаем кнопки улучшений
    improveButtons.clear();
    improveButtonTexts.clear();
    improveDescriptions.clear();

    std::vector<std::string> buttonLabels = {
        "Increase Health",
        "Increase Damage", 
        "Increase Spell Power"
    };
    
    std::vector<std::string> descriptions = {
        "+10 Max Health\nBetter survivability",
        "+1 Damage Power\nStronger attacks",
        "+1 Spell Power\nMore powerful spells"
    };

    int buttonWidth = 300;
    int buttonHeight = 100;
    int buttonSpacing = 20;
    int totalHeight = buttonLabels.size() * buttonHeight + (buttonLabels.size() - 1) * buttonSpacing;
    int startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < buttonLabels.size(); ++i) {
        // Фон кнопки
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition((window.getSize().x - buttonWidth) / 2, 
                          startY + i * (buttonHeight + buttonSpacing));
        
        button.setFillColor(sf::Color(70, 70, 100));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::White);
        improveButtons.push_back(button);

        // Текст кнопки
        sf::Text buttonText;
        buttonText.setFont(font);
        buttonText.setString(buttonLabels[i]);
        buttonText.setCharacterSize(20);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = buttonText.getLocalBounds();
        buttonText.setPosition(
            button.getPosition().x + (buttonWidth - textBounds.width) / 2,
            button.getPosition().y + 15
        );
        improveButtonTexts.push_back(buttonText);

        // Описание улучшения
        sf::Text descriptionText;
        descriptionText.setFont(font);
        descriptionText.setString(descriptions[i]);
        descriptionText.setCharacterSize(14);
        descriptionText.setFillColor(sf::Color(200, 200, 200));
        
        sf::FloatRect descBounds = descriptionText.getLocalBounds();
        descriptionText.setPosition(
            button.getPosition().x + (buttonWidth - descBounds.width) / 2,
            button.getPosition().y + 45
        );
        improveDescriptions.push_back(descriptionText);
    }

    // Рисуем кнопки и текст
    for (const auto& button : improveButtons) {
        window.draw(button);
    }
    for (const auto& text : improveButtonTexts) {
        window.draw(text);
    }
    for (const auto& desc : improveDescriptions) {
        window.draw(desc);
    }

    // Инструкция
    sf::Text instruction;
    instruction.setFont(font);
    instruction.setString("Choose one improvement for the next level");
    instruction.setCharacterSize(18);
    instruction.setFillColor(sf::Color::Yellow);
    instruction.setPosition(
        (window.getSize().x - instruction.getLocalBounds().width) / 2,
        window.getSize().y - 80
    );
    window.draw(instruction);

    window.display();
}