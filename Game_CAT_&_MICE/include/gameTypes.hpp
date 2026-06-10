#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include "gameManager.hpp"
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

struct renderData{
    Player player;
    Enemy enemy;
    Field field;
    std::pair<int, int> towerCoords;
    int currentMoves = 0;
    sf::RenderWindow* window = nullptr;
    LevelAttributes LVLParameters;
    int selectedSpellCard = -1;
    OverlayType currentOverlayType = OverlayType::NONE;
    bool showRangeOverlay = false;

    renderData():player(Player(0)), enemy(Enemy(0, 0)), field(Field(0, 0)){}
};

#endif