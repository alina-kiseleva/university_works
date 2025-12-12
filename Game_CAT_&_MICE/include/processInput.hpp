#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H

#include <SFML/Graphics.hpp>

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,

    ATTACK,
    SELECT_FAR_COMBAT,
    SELECT_CLOSE_COMBAT,
    
    BUY_SPELL,
    USE_SPELL_1,
    USE_SPELL_2,
    USE_SPELL_3,
    USE_SPELL_4,
    
    SAVE_GAME,
    MOUSE_CLICK,
    
    CANCEL_SPELL,
    NONE
};

class ProcessInput{
    CommandType type;
    std::pair<int, int> clickCoords = {-1, -1};
    
public:
    ProcessInput();
    bool processEvent(const sf::Event& event);
    bool processKeyboard(sf::Keyboard::Key key);
    CommandType getCommandType() const;
    std::pair<int, int> getClickCoords() const;
};

#endif