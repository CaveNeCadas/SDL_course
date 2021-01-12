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
Entity* Entitymanager::addEntity(std::string name, LayerType layer )
{
        auto cmp =   std::make_unique< Entity > (this, std::move(name), layer ) ;   
        m_entities.push_back ( std::move(cmp) )  ;
        return cmp.get();
}

Entity* Entitymanager::find(std::string const & name )
{
    for (auto const & ent : m_entities)
    {
        if ( name == ent->getName() ) 
            return ent.get();
    }
    return nullptr;
}