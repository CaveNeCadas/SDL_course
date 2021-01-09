#include "entitymanager.hpp"
#include "entity.hpp"
#include "component.hpp"


Entitymanager::Entitymanager( SDL_Renderer * renderer  )
:m_renderer(renderer)
{

}
Entitymanager::~Entitymanager()
{

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
Entity* Entitymanager::addEntity(std::string name)
{
        auto cmp =   std::make_unique< Entity > (this, std::move(name ) ) ;   
        m_entities.push_back ( std::move(cmp) );
        return m_entities.back().get();
}