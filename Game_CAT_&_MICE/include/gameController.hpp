#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "game.hpp"
#include "processInput.hpp"
#include "visual.hpp"
#include "gui.hpp"

template<class T>
class GameController{
    T inputProcessor;
    VisualDisplay<GameGraphics> GUI;
    sf::RenderWindow window;
    Game& game;
    GameStatus gameStatus;
    renderData renderParameters;

    int windowWidth = 15 * 48 + 200;
    int windowHeight = 15 * 48 + 120;

    bool waitingForTarget = false;

public:
    GameController(Game& currentGame) : 
        inputProcessor(ProcessInput()), game(currentGame), gameStatus(GameStatus::MAIN_MENU),
        GUI(VisualDisplay<GameGraphics>()) {
            window.create(sf::VideoMode(windowWidth, windowHeight), "Cat & mice");
            renderParameters.window = &window;
            renderParameters.towerCoords = game.getGameManager().getTowerCoords();
            updateRenderAttributes();
    }

    void processCommand(){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                window.close();
                break;
            }

            if (inputProcessor.processEvent(event)){
                CommandType commandType = inputProcessor.getCommandType();
                gameStatus = game.getGameStatus();

                if (commandType == CommandType::MOUSE_CLICK){
                    auto [x, y] = inputProcessor.getClickCoords();
                    processClick(x, y);
                }

                if (commandType == CommandType::SAVE_GAME)
                    game.saveGame();

                if (game.getGameStatus() != GameStatus::PLAYING) {continue;}

                bool playerActed = false;

                switch (commandType){
                    case CommandType::MOVE_UP:
                        playerActed = game.movePlayer('w');
                        break;
                    case CommandType::MOVE_DOWN:
                        playerActed = game.movePlayer('s');
                        break;
                    case CommandType::MOVE_LEFT:
                        playerActed = game.movePlayer('a');
                        break;
                    case CommandType::MOVE_RIGHT:
                        playerActed = game.movePlayer('d');
                        break;
                    case CommandType::ATTACK:
                        playerActed = game.attack();
                        break;
                    case CommandType::SELECT_FAR_COMBAT:
                        playerActed = game.changeCombat(typeOfFight::FAR);
                        renderParameters.showRangeOverlay = true;
                        renderParameters.currentOverlayType = OverlayType::COMBAT_RANGE;
                        break;
                    case CommandType::SELECT_CLOSE_COMBAT:
                        playerActed = game.changeCombat(typeOfFight::CLOSE);
                        renderParameters.showRangeOverlay = true;
                        renderParameters.currentOverlayType = OverlayType::COMBAT_RANGE;
                        break;
                    case CommandType::BUY_SPELL:
                        playerActed = game.buySpell();
                        break;
                    case CommandType::USE_SPELL_1:
                        if (game.hasSpell(0)){
                            renderParameters.selectedSpellCard = 0;
                            waitingForTarget = true;
                            renderParameters.showRangeOverlay = true;
                            renderParameters.currentOverlayType = OverlayType::SPELL_RANGE;
                        }
                        break;
                    case CommandType::USE_SPELL_2:
                        if (game.hasSpell(1)){
                            renderParameters.selectedSpellCard = 1;
                            waitingForTarget = true;
                            renderParameters.showRangeOverlay = true;
                            renderParameters.currentOverlayType = OverlayType::SPELL_RANGE;
                        }
                        break;
                    case CommandType::USE_SPELL_3:
                        if (game.hasSpell(2)){
                            renderParameters.selectedSpellCard = 2;
                            waitingForTarget = true;
                            renderParameters.showRangeOverlay = true;
                            renderParameters.currentOverlayType = OverlayType::SPELL_RANGE;
                        }
                        break;
                    case CommandType::USE_SPELL_4:
                        if (game.hasSpell(3)){
                            renderParameters.selectedSpellCard = 3;
                            waitingForTarget = true;
                            renderParameters.showRangeOverlay = true;
                            renderParameters.currentOverlayType = OverlayType::SPELL_RANGE;
                        }
                        break;
                    case CommandType::CANCEL_SPELL:
                        renderParameters.selectedSpellCard = -1;
                        waitingForTarget = false;
                        renderParameters.showRangeOverlay = false;
                        renderParameters.currentOverlayType = OverlayType::NONE;
                        break;
                    default:
                        break;
                }

                if (playerActed){
                    if (game.changesAfterMove()){
                        renderParameters.showRangeOverlay = true;
                        renderParameters.currentOverlayType = OverlayType::TOWER_RANGE;
                    }
                }
                game.checkGameState();
            }
        }
    }

    void processClick(int x, int y){
        if (gameStatus == GameStatus::PLAYING){
            if (!waitingForTarget || renderParameters.selectedSpellCard == -1)
                return;
            if (game.handleMouseClick(x, y, renderParameters.selectedSpellCard)){
                renderParameters.selectedSpellCard = -1;
                waitingForTarget = false;
            }
        } else if (!game.handleMenuClick(x, y)){
            window.close();
        }
    }

    void updateRenderAttributes(){
        renderParameters.player = game.getGameManager().getPlayer();
        renderParameters.enemy = game.getGameManager().getEnemy();
        renderParameters.field = game.getGameManager().getField();
        renderParameters.currentMoves = game.getGameManager().getMoves();
        renderParameters.LVLParameters = game.getLVLParameters();
        renderParameters.towerCoords = game.getGameManager().getTowerCoords();
    }

    void startGame(){
        while (window.isOpen()){
            processCommand();
            updateRenderAttributes();
            gameStatus = game.getGameStatus();
            GUI.render(renderParameters, gameStatus);
            sf::sleep(sf::milliseconds(16));
        }
    }
};

#endif