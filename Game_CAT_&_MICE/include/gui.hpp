#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>

enum class GameStatus{
    VICTORY,
    PLAYING,
    DEFEAT,
    MAIN_MENU,
    IMPROVE
};

enum class OverlayType{
    NONE,
    SPELL_RANGE,
    TOWER_RANGE,
    COMBAT_RANGE
};

class GameGraphics{
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

    int CELL_SIZE = 48;
    int CARD_WIDTH = 120;
    int CARD_HEIGHT = 80;
    int CARD_MARGIN = 10;
    int CARDS_Y_POSITION; 

    std::vector<sf::RectangleShape> cardBackgrounds;
    std::vector<sf::Text> cardTitles;
    std::vector<sf::Text> cardDescriptions;

    std::vector<sf::RectangleShape> rangeOverlay;

    sf::Text menuTitle;
    sf::Text levelCompleteTitle;
    std::vector<sf::RectangleShape> menuButtons;
    std::vector<sf::Text> menuButtonTexts;
    std::vector<sf::RectangleShape> levelCompleteButtons;
    std::vector<sf::Text> levelCompleteButtonTexts;
    std::vector<sf::RectangleShape> gameOverButtons;
    std::vector<sf::Text> gameOverButtonTexts;

    sf::Text improveTitle;
    std::vector<sf::RectangleShape> improveButtons;
    std::vector<sf::Text> improveButtonTexts;
    std::vector<sf::Text> improveDescriptions;

public:
    GameGraphics(int length, int width);
    void updateCellSize(int newSize);
    void updateRangeOverlay(GameManager gameManager, OverlayType currentOverlayType, bool showRangeOverlay, int selectedSpellCard);
    void showSpellRange(GameManager gameManager, int range);
    void showTowerRange(GameManager gameManager);
    void showCombatRange(GameManager gameManager);
    void clearRangeOverlay();
    void loadAssets();
    void initializeSpellCards(Player player, Field field);
    void update(GameManager gameManager, int selectedSpellCard, sf::RenderWindow& window, OverlayType currentOverlayType, bool showRangeOverlay, LevelAttributes lvl);
    void render(GameManager gameManager, sf::RenderWindow& window, GameStatus gameStatus);
    void renderMainMenu(sf::RenderWindow& window, GameStatus gameStatus);
    void renderLevelComplete(sf::RenderWindow& window, const GameManager& gameManager, int goalScore);
    void renderGameOver(sf::RenderWindow& window, const GameManager& gameManager, LevelAttributes lvl);
    void initializeMenuUI();
    void renderImprove(sf::RenderWindow& window, const GameManager& gameManager, int playerHP);
    void initializeImproveUI();
};

#endif