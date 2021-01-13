#ifndef COLLIDER_COMPONENT_OF_THE_GAME_HPP
#define COLLIDER_COMPONENT_OF_THE_GAME_HPP

#include "graphicshdr.hpp"
#include "component.hpp"
#include "components/transformComponent.hpp"
#include "game.hpp"

class ColliderComponent : public Component
{
    private:
        std::string m_tag;
        SDL_Rect m_collider;
        SDL_Rect m_src_rect;
        SDL_Rect m_dst_rect;
        TransformComponent* m_transform;
    public:

        ColliderComponent( Entity* owner,std::string tag, int x, int y, int w, int h)
        : Component(owner)
        , m_tag(std::move(tag))
        , m_collider{ x, y, w, h}
        , m_src_rect{0,0,0,0}
        , m_dst_rect{0,0,0,0}
        , m_transform{nullptr}
        {/*NOP*/}   

        void initialize () override 
        {
            if ( !m_owner->hasComponent<TransformComponent>() )
                return;
            m_transform = m_owner->getComponent<TransformComponent>();
            m_src_rect = {0,0, m_transform->getWidth(), m_transform->getHeight()};
            m_dst_rect = {m_collider.x,m_collider.y, m_collider.w, m_collider.h };
        }
        
        void update (float deltaTime) override
        {
            if (nullptr == m_transform)
                return;
            m_collider.x = m_transform->getPosition().x;
            m_collider.y = m_transform->getPosition().y;
            m_collider.w = m_transform->getWidth() * m_transform->getScale();
            m_collider.h = m_transform->getHeight() * m_transform->getScale();
            m_dst_rect.x = m_collider.x - Game::s_camera.x;
            m_dst_rect.y = m_collider.y - Game::s_camera.y;
        } 

        void render(  SDL_Renderer * a_renderer ) override
        {
        }

        std::string getTag() const { return m_tag;} 

        SDL_Rect const & getCollider() const  { return m_collider;} 
};

#endif