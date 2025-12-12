#ifndef VISUAL_H
#define VISUAL_H

#include "gameTypes.hpp"

template<class RenderType>
class VisualDisplay{
    RenderType renderer;
public:
    VisualDisplay():renderer(RenderType()){}
    void render(renderData renderParameters, GameStatus status){
        renderer.render(renderParameters, status);
    }
};

#endif