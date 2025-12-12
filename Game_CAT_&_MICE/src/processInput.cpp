#include "../include/processInput.hpp"

ProcessInput::ProcessInput():type(CommandType::NONE){}

bool ProcessInput::processEvent(const sf::Event& event){
    if (event.type == sf::Event::KeyPressed)
        return processKeyboard(event.key.code);

    else if (event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left){
            type = CommandType::MOUSE_CLICK;
            clickCoords = {event.mouseButton.x, event.mouseButton.y};
            return true;
        }
    }

    return false;
}

bool ProcessInput::processKeyboard(sf::Keyboard::Key key){
    switch (key) {
        case sf::Keyboard::W:
            type = CommandType::MOVE_UP;
            break;
        case sf::Keyboard::A:
            type = CommandType::MOVE_LEFT;
            break;
        case sf::Keyboard::S:
            type = CommandType::MOVE_DOWN;
            break;
        case sf::Keyboard::D:
            type = CommandType::MOVE_RIGHT;
            break;
        case sf::Keyboard::Q:
            type = CommandType::SELECT_FAR_COMBAT;
            break;
        case sf::Keyboard::E:
            type = CommandType::SELECT_CLOSE_COMBAT;
            break;
        case sf::Keyboard::F:
            type = CommandType::ATTACK;
            break;
        case sf::Keyboard::R:
            type = CommandType::BUY_SPELL;
            break;
        case sf::Keyboard::L:
            type = CommandType::SAVE_GAME;
            break;
        case sf::Keyboard::Num1:
            type = CommandType::USE_SPELL_1;
            break;    
        case sf::Keyboard::Num2:
            type = CommandType::USE_SPELL_2;
            break;   
        case sf::Keyboard::Num3:
            type = CommandType::USE_SPELL_3;
            break;
        case sf::Keyboard::Num4:
            type = CommandType::USE_SPELL_4;
            break;
        case sf::Keyboard::Escape:
            type = CommandType::CANCEL_SPELL;
            break;
        default:
            type = CommandType::NONE;
            break;
    }
    return (type != CommandType::NONE) ? true : false;
}

CommandType ProcessInput::getCommandType() const{
    return type;
}

std::pair<int, int> ProcessInput::getClickCoords() const{
    return clickCoords;
}