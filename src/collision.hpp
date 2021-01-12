#ifndef COLLISION_OF_ENTITIES_THE_GAME_HPP
#define COLLISION_OF_ENTITIES_THE_GAME_HPP

#include "graphicshdr.hpp"


class Collision
{
    public:
        static bool checkRectangleCollision(SDL_Rect const & rectA, SDL_Rect const & rectB );
};


#endif