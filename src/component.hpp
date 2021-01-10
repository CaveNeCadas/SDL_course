#ifndef COMPONENT_OF_THE_GAME_HPP
#define COMPONENT_OF_THE_GAME_HPP

#include "graphicshdr.hpp"


class Entity;

class Component
{
    protected:

        Entity* m_owner;

    public:

        explicit Component(Entity* owner):m_owner(owner){}
        virtual ~Component(){}
        virtual void initialize () = 0 ;
        virtual void update (float deltaTime) = 0;
        virtual void render(  SDL_Renderer * ) = 0;
        
};

#endif