#ifndef GAME_H
#define GAME_H

#include "gameManager.hpp"
#include "saveManager.hpp"
#include "gameTypes.hpp"

class Game{
    GameManager gameManager;
    GameStatus gameStatus;
    SaveManager saver;

    LevelAttributes levelParameters;
    bool goToNextLVL = 0;

    int windowWidth = 15 * 48 + 200;
    int windowHeight = 15 * 48 + 120;

    int freezed = 1;

    bool isTowerAttack = false;
    int timeWithoutAttack = 0;

public:
    Game();
    void start();
    void checkGameState();
    void saveGame();
    void loadGame();
    void handleInput();

    bool movePlayer(char direction);
    bool attack();
    bool changeCombat(typeOfFight type);
    bool buySpell();
    bool hasSpell(int position);
    bool changesAfterMove();

    bool handleMenuClick(int mouseX, int mouseY);
    bool handleMouseClick(int mouseX, int mouseY, int selectedSpellCard);
    void nextLevel();
    bool checkButton(int mouseX, int mouseY, int btnX, int btnY, int btnWidth, int btnHeight);

    GameStatus getGameStatus() const;
    LevelAttributes getLVLParameters() const;
    GameManager getGameManager() const;
};

#endif