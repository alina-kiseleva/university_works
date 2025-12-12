#include "../include/gui.hpp"

GameGraphics::GameGraphics(){
    loadAssets();

    int length = 15;
    int width = 15;

    CARDS_Y_POSITION = length * CELL_SIZE + 20;

    infoText.setFont(font);
    infoText.setCharacterSize(14);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition(width * CELL_SIZE + 10, 10);

    instructionText.setFont(font);
    instructionText.setCharacterSize(14);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(width * CELL_SIZE + 10, 310);

    menuTitle.setFont(fontResult);
    menuTitle.setString("CAT & MICE");
    menuTitle.setCharacterSize(64);
    menuTitle.setFillColor(sf::Color::Yellow);
    menuTitle.setStyle(sf::Text::Bold);

    levelCompleteTitle.setFont(fontResult);
    levelCompleteTitle.setString("LEVEL COMPLETED!");
    levelCompleteTitle.setCharacterSize(48);
    levelCompleteTitle.setFillColor(sf::Color::Green);
    levelCompleteTitle.setStyle(sf::Text::Bold);

    gameOverText.setFont(fontResult);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setStyle(sf::Text::Bold);

    improveTitle.setFont(fontResult);
    improveTitle.setString("CHOOSE IMPROVEMENT");
    improveTitle.setCharacterSize(48);
    improveTitle.setFillColor(sf::Color::Yellow);
    improveTitle.setStyle(sf::Text::Bold);
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

void GameGraphics::render(renderData renderParameters, GameStatus gameStatus){
    updateCellSize(renderParameters.LVLParameters.cellSize);
    initializeSpellCards(renderParameters);
    switch (gameStatus) {
        case GameStatus::MAIN_MENU:
            renderMainMenu(renderParameters.window, gameStatus);
            break;
        case GameStatus::IMPROVE:
            renderImprove(renderParameters);
            break;
        case GameStatus::PLAYING:
            update(renderParameters);
            renderPlaying(renderParameters, gameStatus);
            break;
        case GameStatus::VICTORY:
            renderLevelComplete(renderParameters);
            break;
        case GameStatus::DEFEAT:
            renderGameOver(renderParameters);
            break;
    }
}

void GameGraphics::initializeSpellCards(renderData renderParameters) {
    cardBackgrounds.clear();
    cardTitles.clear();
    cardDescriptions.clear();

    int cardCount = renderParameters.player.getHandSize();

    int totalWidth = renderParameters.field.getWidth() * CELL_SIZE;
    int cardsTotalWidth = cardCount * CARD_WIDTH + (cardCount - 1) * CARD_MARGIN;
    int startX = (totalWidth - cardsTotalWidth) / 2;

    for (int i = 0; i < cardCount; ++i) {
        sf::RectangleShape cardBackground(sf::Vector2f(CARD_WIDTH, CARD_HEIGHT));
        cardBackground.setPosition(startX + i * (CARD_WIDTH + CARD_MARGIN), CARDS_Y_POSITION);
        cardBackground.setFillColor(sf::Color(50, 50, 80));
        cardBackground.setOutlineThickness(2);
        cardBackground.setOutlineColor(sf::Color::White);
        cardBackgrounds.push_back(cardBackground);

        sf::Text cardTitle;
        cardTitle.setFont(font);
        cardTitle.setCharacterSize(12);
        cardTitle.setFillColor(sf::Color::Yellow);
        cardTitle.setPosition(startX + i * (CARD_WIDTH + CARD_MARGIN) + 5, CARDS_Y_POSITION + 5);

        sf::Text cardDescription;
        cardDescription.setFont(font);
        cardDescription.setCharacterSize(10);
        cardDescription.setFillColor(sf::Color::White);
        cardDescription.setPosition(startX + i * (CARD_WIDTH + CARD_MARGIN) + 5, CARDS_Y_POSITION + 25);

        cardTitles.push_back(cardTitle);
        cardDescriptions.push_back(cardDescription);
    }
}

void GameGraphics::update(renderData renderParameters) {
    Player player = renderParameters.player;
    Enemy enemy = renderParameters.enemy;
    int score = player.getScore();
    int moves = renderParameters.currentMoves;

    updateRangeOverlay(renderParameters);

    for (int i = 0; i < cardBackgrounds.size(); ++i) {
        if (i == renderParameters.selectedSpellCard) {
            cardBackgrounds[i].setOutlineColor(sf::Color::Yellow);
            cardBackgrounds[i].setOutlineThickness(4);
        } else {
            cardBackgrounds[i].setOutlineColor(sf::Color::White);
            cardBackgrounds[i].setOutlineThickness(2);
        }

        if (player.hasSpell(i)) {
            spellType type = player.getSpellName(i);
            std::string title, description;

            switch (type) {
                case spellType::DIRECT_DAMAGE_SPELL:
                    title = "Direct Damage";
                    description = "Single target\nRange: 3\nDamage: " + std::to_string(2*renderParameters.LVLParameters.spellDamageKoef);
                    cardBackgrounds[i].setFillColor(sf::Color(120, 50, 50));
                    break;
                case spellType::AREA_DAMAGE_SPELL:
                    title = "Area Damage";
                    description = "2x2 area\nRange: 4\nDamage: " + std::to_string(renderParameters.LVLParameters.spellDamageKoef);
                    cardBackgrounds[i].setFillColor(sf::Color(50, 120, 50));
                    break;
                case spellType::TRAP_SPELL:
                    title = "Trap";
                    description = "Place trap\nEnemy takes\ndamage " + std::to_string(renderParameters.LVLParameters.spellDamageKoef) + " on step";
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
        "Score: " + std::to_string(score) + "/" + std::to_string(renderParameters.LVLParameters.goalScore) + "\n" +
        "Coins: " + std::to_string(player.getCoins()) + "\n" +
        "Moves: " + std::to_string(moves) + "/" + std::to_string(renderParameters.LVLParameters.goalMoves) + "\n" +
        "Damage: " + (player.getCombatType() == typeOfFight::CLOSE ? "Close(" + std::to_string(2*renderParameters.LVLParameters.playerDamageKoef) + ")" : 
        "Far(" + std::to_string(renderParameters.LVLParameters.playerDamageKoef) + ")" ) + "\n" +
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
        "Defeat " + std::to_string(renderParameters.LVLParameters.goalScore) + " enemies\n"
        "in under " + std::to_string(renderParameters.LVLParameters.goalMoves) + " moves!"
    );
}

void GameGraphics::renderPlaying(renderData renderParameters, GameStatus gameStatus) {
    sf::RenderWindow& window = *renderParameters.window;
    window.clear(sf::Color::Black);

    Field field = renderParameters.field;
    int length = field.getLength();
    int width = field.getWidth();

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

            } else if (cellChar == character::ENEMY) {
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

    for (auto& card : cardBackgrounds) {
        window.draw(card);
    }
    for (auto& title : cardTitles) {
        window.draw(title);
    }
    for (auto& description : cardDescriptions) {
        window.draw(description);
    }

    // Отрисовка фона боковой панели
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

void GameGraphics::updateRangeOverlay(renderData renderParameters) {
    rangeOverlay.clear();

    if (!renderParameters.showRangeOverlay) return;

    Player player = renderParameters.player;
    auto [playerX, playerY] = player.getCoordinates();

    sf::Color overlayColor;

    switch (renderParameters.currentOverlayType) {
        case OverlayType::SPELL_RANGE:
            if (renderParameters.selectedSpellCard != -1 && player.hasSpell(renderParameters.selectedSpellCard)) {
                spellType type = player.getSpellName(renderParameters.selectedSpellCard);
                if (type == spellType::DIRECT_DAMAGE_SPELL){
                    showSpellRange(renderParameters.player, renderParameters.field, 3);
                } else if (type == spellType::AREA_DAMAGE_SPELL){
                    showSpellRange(renderParameters.player, renderParameters.field, 4);
                }
            }
            break;

        case OverlayType::TOWER_RANGE:
            showTowerRange(renderParameters.field, renderParameters.towerCoords);
            break;

        case OverlayType::COMBAT_RANGE:
            showCombatRange(renderParameters.player, renderParameters.field);
            break;

        case OverlayType::NONE:
            break;
    }
}

void GameGraphics::showSpellRange(Player player, Field field, int range) {
    auto [playerX, playerY] = player.getCoordinates();

    for (int i = 0; i < field.getLength(); ++i) {
        for (int j = 0; j < field.getWidth(); ++j) {
            int distance = std::abs(playerX - i) + std::abs(playerY - j);
            if (distance <= range && field.canMoveTo(i, j)) {
                sf::RectangleShape overlay(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                overlay.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                overlay.setFillColor(sf::Color(128, 0, 128, 100));
                rangeOverlay.push_back(overlay);
            }
        }
    }
}

void GameGraphics::showTowerRange(Field field, std::pair<int, int> towerCoords) {
    auto [towerX, towerY] = towerCoords;
    int towerRange = 3;

    for (int i = 0; i < field.getLength(); ++i) {
        for (int j = 0; j < field.getWidth(); ++j) {
            int distance = std::abs(towerX - i) + std::abs(towerY - j);
            if (distance <= towerRange) {
                sf::RectangleShape overlay(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                overlay.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                overlay.setFillColor(sf::Color(255, 0, 0, 80));
                rangeOverlay.push_back(overlay);
            }
        }
    }
}

void GameGraphics::showCombatRange(Player player, Field field) {
    auto [playerX, playerY] = player.getCoordinates();
    int range = (player.getCombatType() == typeOfFight::CLOSE) ? 1 : 2;

    for (int i = 0; i < field.getLength(); ++i) {
        for (int j = 0; j < field.getWidth(); ++j) {
            int distance = std::abs(playerX - i) + std::abs(playerY - j);
            if (distance <= range) {
                sf::RectangleShape overlay(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                overlay.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                overlay.setFillColor(sf::Color(0, 0, 255, 80));
                rangeOverlay.push_back(overlay);
            }
        }
    }
}

void GameGraphics::renderMainMenu(sf::RenderWindow* wndw, GameStatus gameStatus) {
    sf::RenderWindow& window = *wndw;
    window.clear(sf::Color(30, 30, 50));

    sf::FloatRect titleBounds = menuTitle.getLocalBounds();
    menuTitle.setPosition((window.getSize().x - titleBounds.width) / 2, 100);
    window.draw(menuTitle);

    menuButtons.clear();
    menuButtonTexts.clear();

    std::vector<std::string> buttonLabels = {"New Game", "Continue", "Exit"};
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonSpacing = 20;
    int totalHeight = buttonLabels.size() * buttonHeight + (buttonLabels.size() - 1) * buttonSpacing;
    int startY = (window.getSize().y - totalHeight) / 2;

    for (size_t i = 0; i < buttonLabels.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition((window.getSize().x - buttonWidth) / 2, 
                          startY + i * (buttonHeight + buttonSpacing));
        button.setFillColor(sf::Color(70, 70, 100));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::White);
        menuButtons.push_back(button);

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

    for (const auto& button : menuButtons) {
        window.draw(button);
    }
    for (const auto& text : menuButtonTexts) {
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

void GameGraphics::renderLevelComplete(renderData renderParameters) {
    sf::RenderWindow& window = *renderParameters.window;
    window.clear(sf::Color(30, 50, 30));

    Player player = renderParameters.player;

    sf::FloatRect titleBounds = levelCompleteTitle.getLocalBounds();
    levelCompleteTitle.setPosition((window.getSize().x - titleBounds.width) / 2, 150);
    window.draw(levelCompleteTitle);

    sf::Text statsText;
    statsText.setFont(font);
    statsText.setString(
        "Score: " + std::to_string(player.getScore()) + "/" + std::to_string(renderParameters.LVLParameters.goalScore) + "\n" +
        "Moves: " + std::to_string(renderParameters.currentMoves) + "\n" +
        "Coins: " + std::to_string(player.getCoins()) + "\n" +
        "Health: " + std::to_string(player.getHealth())
    );
    statsText.setCharacterSize(24);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition((window.getSize().x - statsText.getLocalBounds().width) / 2, 250);
    window.draw(statsText);

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
        (window.getSize().x - saveInfo.getLocalBounds().width) / 2, window.getSize().y - 50);
    window.draw(saveInfo);
    window.display();
}

void GameGraphics::renderGameOver(renderData renderParameters) {
    sf::RenderWindow& window = *renderParameters.window;
    window.clear(sf::Color(50, 30, 30));

    Player player = renderParameters.player;

    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(
        (window.getSize().x - gameOverBounds.width) / 2, 150);
    window.draw(gameOverText);

    sf::Text reasonText;
    reasonText.setFont(font);
    reasonText.setCharacterSize(24);
    reasonText.setFillColor(sf::Color::White);

    std::string reason = "Out of moves!";
    if (!player.isAlive()) reason = "Player died!";
    reasonText.setString(reason);

    reasonText.setPosition((window.getSize().x - reasonText.getLocalBounds().width) / 2, 250);
    window.draw(reasonText);

    sf::Text statsText;
    statsText.setFont(font);
    statsText.setString(
        "Score: " + std::to_string(player.getScore()) + "/" + std::to_string(renderParameters.LVLParameters.goalScore) + "\n" +
        "Moves: " + std::to_string(renderParameters.currentMoves) + "/" + std::to_string(renderParameters.LVLParameters.goalMoves) + "\n" +
        "Coins: " + std::to_string(player.getCoins())
    );
    statsText.setCharacterSize(20);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition((window.getSize().x - statsText.getLocalBounds().width) / 2, 300);
    window.draw(statsText);

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

    window.draw(button);
    window.draw(buttonText);

    sf::Text saveInfo;
    saveInfo.setFont(font);
    saveInfo.setString("Press 'L' to save progress");
    saveInfo.setCharacterSize(16);
    saveInfo.setFillColor(sf::Color::White);
    saveInfo.setPosition(
        (window.getSize().x - saveInfo.getLocalBounds().width) / 2,
        window.getSize().y - 50);
    window.draw(saveInfo);
    window.display();
}

void GameGraphics::renderImprove(renderData renderParameters) {
    sf::RenderWindow& window = *renderParameters.window;
    window.clear(sf::Color(30, 30, 50));

    Player player = renderParameters.player;

    sf::FloatRect titleBounds = improveTitle.getLocalBounds();
    improveTitle.setPosition((window.getSize().x - titleBounds.width) / 2, 100);
    window.draw(improveTitle);

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
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition((window.getSize().x - buttonWidth) / 2, 
                          startY + i * (buttonHeight + buttonSpacing));
        
        button.setFillColor(sf::Color(70, 70, 100));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::White);
        improveButtons.push_back(button);

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

    for (const auto& button : improveButtons) {
        window.draw(button);
    }
    for (const auto& text : improveButtonTexts) {
        window.draw(text);
    }
    for (const auto& desc : improveDescriptions) {
        window.draw(desc);
    }

    sf::Text instruction;
    instruction.setFont(font);
    instruction.setString("Choose one improvement for the next level");
    instruction.setCharacterSize(18);
    instruction.setFillColor(sf::Color::Yellow);
    instruction.setPosition(
        (window.getSize().x - instruction.getLocalBounds().width) / 2, window.getSize().y - 80);
    window.draw(instruction);
    window.display();
}