#ifndef ENTITY_OF_THE_GAME_HPP
#define ENTITY_OF_THE_GAME_HPP

#include <vector>
#include <string>
#include <memory>

#include "graphicshdr.hpp"


class Component;
class Entitymanager;

class Entity
{
    private:

        Entitymanager* m_manager;
        bool m_isActive;
        std::string m_name;
        std::vector<Component *> m_compponents;

    public:

        Entity(Entitymanager* manager, std::string name);
        virtual ~Entity();
        void update (float deltaTime);
        void render();
        void destroy();
        bool isActive () const { return m_isActive; }
};



#endif