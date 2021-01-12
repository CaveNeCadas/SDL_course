#include "entity.hpp"

#include "entitymanager.hpp"

#include "component.hpp"

Entity::Entity(Entitymanager* manager, std::string name , LayerType layer)
: m_manager(manager)
, m_isActive{true}
, m_name(std::move(name))
, m_layer(layer)
{

}

Entity::~Entity()
{

}

void Entity::initialize()
{
    for (auto & comp : m_compponents)
    {
        comp-> initialize ();
    }
}

void Entity::update (float deltaTime)
{
    for (auto & comp : m_compponents)
    {
        comp-> update (deltaTime);
    }
}

void Entity::render(  )
{
    for (auto & comp : m_compponents)
    {
        comp-> render ( m_manager->getRenderer() );
    }
}

void Entity::destroy()
{
    m_isActive = false;   
}
