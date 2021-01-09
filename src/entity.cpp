#include "entity.hpp"

#include "entitymanager.hpp"

#include "component.hpp"

Entity::Entity(Entitymanager* manager, std::string name)
: m_manager(manager)
, m_isActive{true}
, m_name(std::move(name))
{

}

Entity::~Entity()
{

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
