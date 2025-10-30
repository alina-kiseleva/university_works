#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "gameManager.hpp"

class Game {
    sf::RenderWindow window;
    GameManager gameManager;

    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture defaultCellTexture;
    sf::Texture obstacleCellTexture;
    sf::Texture freezeCellTexture;
    sf::Texture trapCellTexture;
    sf::Texture towerTexture;

    sf::Font font;
    sf::Font fontResult;
    sf::Text infoText;
    sf::Text instructionText;
    sf::Text victoryText;
    sf::Text gameOverText;
    
    bool gameWon;
    bool gameLost;

    int selectedSpellCard = -1;
    bool waitingForTarget = false;

    bool isTowerAttack = false;
    int timeWithoutAttack = 0;

    const int CELL_SIZE = 48;
    int CARD_WIDTH = 120;
    int CARD_HEIGHT = 80;
    int CARD_MARGIN = 10;
    int CARDS_Y_POSITION; 

    std::vector<sf::RectangleShape> cardBackgrounds;
    std::vector<sf::Text> cardTitles;
    std::vector<sf::Text> cardDescriptions;

    std::vector<sf::RectangleShape> rangeOverlay;
    bool showRangeOverlay = false;
    enum class OverlayType {
        NONE,
        SPELL_RANGE,
        TOWER_RANGE,
        COMBAT_RANGE
    } currentOverlayType = OverlayType::NONE;

    void updateRangeOverlay();
    void showSpellRange(int range);
    void showTowerRange();
    void showCombatRange();
    void clearRangeOverlay();

    void loadAssets();
    void handleInput();
    void handleMouseClick(int mouseX, int mouseY);
    void initializeSpellCards();
    void update();
    void render();

public:
    Game();
    void run();
    void checkGameState();
};

#endif