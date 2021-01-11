#ifndef ENTITY_MANAGER_OF_THE_GAME_HPP
#define ENTITY_MANAGER_OF_THE_GAME_HPP

#include <vector>
#include <string>
#include <memory>

#include "graphicshdr.hpp"
#include "entity.hpp"

class Entity;

class Entitymanager
{
    private:

        SDL_Renderer * m_renderer;
        std::vector<  std::unique_ptr< Entity > > m_entities;

    public:

        explicit Entitymanager(  SDL_Renderer * renderer );
        virtual ~Entitymanager();
        void initialize();
        void update (float deltaTime);
        void render();
        void clear();
        SDL_Renderer*  getRenderer () { return m_renderer; }  
        Entity* addEntity(std::string name);
        std::size_t count () const {   return m_entities.size(); }
        bool empty () const { return m_entities.empty(); }
};



#endif