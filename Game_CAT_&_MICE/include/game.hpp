#ifndef GAME_H
#define GAME_H

#include "gameManager.hpp"
#include "gui.hpp"
#include "saveManager.hpp"

class Game{
    sf::RenderWindow window;
    GameManager gameManager;
    GameStatus gameStatus;
    GameGraphics GUI;
    SaveManager saver;

    LevelAttributes levelParameters;
    bool goToNextLVL = 0;

    int windowWidth;
    int windowHeight;

    int selectedSpellCard = -1;
    bool waitingForTarget = false;

    bool isTowerAttack = false;
    int timeWithoutAttack = 0;

    int CELL_SIZE = 48;
    OverlayType currentOverlayType = OverlayType::NONE;
    bool showRangeOverlay;

public:
    Game();
    void start();
    void checkGameState();
    void saveGame();
    // bool checkSaveData(SaveData data);
    // int makeHash(SaveData data);
    // int mix(int hash, int value);
    void loadGame();
    void handleInput();
    void handleMenuClick(int mouseX, int mouseY);
    void handleMouseClick(int mouseX, int mouseY);
    void nextLevel();
    bool checkButton(int mouseX, int mouseY, int btnX, int btnY, int btnWidth, int btnHeight);
};

#endif