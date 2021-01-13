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
        ent-> initialize();
    }
}

void Entitymanager::update (float deltaTime)
{
    for (auto & ent : m_entities)
    {
        ent-> update(deltaTime);
    }
}
void Entitymanager::render()
{
    for (auto & ent : m_entities)
    {
        ent-> render();
    }
}
void Entitymanager::clear()
{
    for (auto & ent : m_entities)
    {
        ent-> destroy();
    }

}
Entity* Entitymanager::addEntity( uint32_t entId, LayerType layer )
{
    auto cmp = std::make_unique< Entity > (this, entId, layer ) ;   
    m_entities.push_back(std::move(cmp)) ;
    return m_entities.back().get();
}

Entity* Entitymanager::find( uint32_t entId  )
{
    auto ent_itr =  std::find_if ( std::begin(m_entities), std::end(m_entities), [this, entId](auto & ptr) { return ptr->getId() == entId;} );
    if (  ent_itr != m_entities.end() )
    {
            return ent_itr->get();
    }
    return nullptr;
}

uint32_t Entitymanager::checkCollision(Entity* entity)
{
    if (!entity->hasComponent<ColliderComponent>())
        return 0;

    auto thscollider = entity->getComponent<ColliderComponent>() ;

    for (auto & ptrent : m_entities)
    {
           if ( entity->getId() != ptrent->getId() && ptrent->hasComponent<ColliderComponent>())
           {
                ColliderComponent * collider = ptrent->getComponent<ColliderComponent>();
                
                if (Collision::checkRectangleCollision( thscollider->getCollider(), collider->getCollider() ) )
                {
                    //spdlog::warn ("collision detected...");
                    return ptrent->getId();
                }
           }
    }
     

    return 0;
}