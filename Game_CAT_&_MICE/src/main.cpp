#include "../include/game.hpp"
#include "../include/gameController.hpp"
#include "../include/processInput.hpp"

int main(){
    Game game;
    GameController<ProcessInput> gameController(game);
    gameController.startGame();
    return 0;
}