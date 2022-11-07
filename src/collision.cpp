#include "collision.hpp"

bool Collision::checkRectangleCollision(SDL_Rect const & rectA, SDL_Rect const & rectB )
{
    return   rectA.x + rectA.w >= rectB.x 
           && 
             rectA.y + rectA.h >= rectB.y 
           &&
             rectB.x + rectB.w >= rectA.x 
           && 
             rectB.y + rectB.h >= rectA.y
            ;

}