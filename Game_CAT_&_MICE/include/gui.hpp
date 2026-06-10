#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include "gameTypes.hpp"

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
    sf::Text gameOverText;
    sf::Text menuTitle;
    sf::Text levelCompleteTitle;
    sf::Text improveTitle;

    int CELL_SIZE = 48;
    int CARD_WIDTH = 120;
    int CARD_HEIGHT = 80;
    int CARD_MARGIN = 10;
    int CARDS_Y_POSITION; 

    std::vector<sf::RectangleShape> cardBackgrounds;
    std::vector<sf::Text> cardTitles;
    std::vector<sf::Text> cardDescriptions;
    std::vector<sf::RectangleShape> rangeOverlay;
    std::vector<sf::RectangleShape> menuButtons;
    std::vector<sf::Text> menuButtonTexts;
    std::vector<sf::RectangleShape> levelCompleteButtons;
    std::vector<sf::Text> levelCompleteButtonTexts;
    std::vector<sf::RectangleShape> gameOverButtons;
    std::vector<sf::Text> gameOverButtonTexts;
    std::vector<sf::RectangleShape> improveButtons;
    std::vector<sf::Text> improveButtonTexts;
    std::vector<sf::Text> improveDescriptions;

public:
    GameGraphics();
    void loadAssets();
    void render(renderData renderParameters, GameStatus gameStatus);
    void initializeSpellCards(renderData renderParameters);
    void update(renderData renderParameters);
    void renderPlaying(renderData renderParameters);
    void renderMainMenu(sf::RenderWindow* wndw);
    void renderLevelComplete(renderData renderParameters);
    void renderGameOver(renderData renderParameters);
    void renderImprove(renderData renderParameters);
    void updateCellSize(int newSize);
    void updateRangeOverlay(renderData renderParameters);
    void showSpellRange(Player player, Field field, int range);
    void showTowerRange(Field field, std::pair<int, int> towerCoords);
    void showCombatRange(Player player, Field field);
};

#endif