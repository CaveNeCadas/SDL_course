#include "entitymanager.hpp"
#include "entity.hpp"
#include "component.hpp"
#include "components/colliderComponent.hpp"
#include "collision.hpp"


Entitymanager::Entitymanager( SDL_Renderer * renderer  )
:m_renderer(renderer)
{

}
Entitymanager::~Entitymanager()
{

}

void Entitymanager::initialize()
{
    for (auto & ent : m_entities)
    {
        ent.second-> initialize();
    }
}

void Entitymanager::update (float deltaTime)
{
    for (auto & ent : m_entities)
    {
        ent.second-> update(deltaTime);
    }
}
void Entitymanager::render()
{
    for (auto & ent : m_entities)
    {
        ent.second-> render();
    }
}
void Entitymanager::clear()
{
    for (auto & ent : m_entities)
    {
        ent.second-> destroy();
    }

}
Entity* Entitymanager::addEntity( uint32_t entId, LayerType layer )
{
    auto cmp = std::make_unique< Entity > (this, entId, layer ) ;   
    m_entities[entId] = std::move(cmp);
    return m_entities[entId].get();
}

Entity* Entitymanager::find( uint32_t entId  )
{
    auto ent_itr =  m_entities.find(entId);
    if (  ent_itr != m_entities.end() )
    {
            return ent_itr->second.get();
    }
    return nullptr;
}

uint32_t Entitymanager::checkCollision(Entity* entity)
{
    if (entity->hasComponent<ColliderComponent>())
        return 0;

    auto thscollider = entity->getComponent<ColliderComponent>() ;

    for (auto & ptrent : m_entities)
    {
           if ( entity->getId() != ptrent.second->getId() &&
                entity->getId() != hash("Tile") && 
                ptrent.second->hasComponent<ColliderComponent>())
           {
                ColliderComponent * collider = ptrent.second->getComponent<ColliderComponent>();
                
                if (Collision::checkRectangleCollision( thscollider->getCollider(), collider->getCollider() ) )
                {
                    spdlog::warn ("collision detected...");
                    return ptrent.second->getId();
                }
           }
    }
     

    return 0;
}