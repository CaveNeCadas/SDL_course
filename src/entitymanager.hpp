#ifndef ENTITY_MANAGER_OF_THE_GAME_HPP
#define ENTITY_MANAGER_OF_THE_GAME_HPP

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
		
#include "graphicshdr.hpp"
#include "entity.hpp"
#include "constants.hpp"
class Entity;

class Entitymanager
{
    private:

        SDL_Renderer * m_renderer;
        std::vector< entity_ptr_t > m_entities;
        //std::unordered_map<uint32_t, entity_ptr_t > m_entities; 
    public:

        explicit Entitymanager(  SDL_Renderer * renderer );
        virtual ~Entitymanager();
        void initialize();
        void update (float deltaTime);
        void render();
        void clear();
        SDL_Renderer*  getRenderer () { return m_renderer; }  
        Entity* addEntity(  uint32_t entId,  LayerType layer = LayerType::DEFAULT_LAYER );
        std::size_t count () const {   return m_entities.size(); }
        bool empty () const { return m_entities.empty(); }
        Entity* find( uint32_t entId );
        uint32_t checkCollision(Entity* entity);
};



#endif