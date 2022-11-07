#pragma once

#include "graphicshdr.hpp"
#include "component.hpp"
#include "entity.hpp"
#include "components/transformComponent.hpp"

class ProjectileEmitterComponent : public Component
{
    private:
        TransformComponent* m_transform;
        
        
        vec2_t m_origin;
        int32_t m_speed;
        int32_t m_range;
        float   m_angle;
        bool    m_shoot_loop;
    public:

        ProjectileEmitterComponent( Entity* owner,  int speed, float angle, int rg,  bool loop)
        : Component(owner)
        , m_transform{nullptr}

        #ifdef GAME_AVX
            , m_origin{ ._coords = { _mm_set1_epi32(0)}}
         #else
            , m_origin(0, 0)
         #endif   
        
        , m_speed(speed)
        , m_range(rg)
        , m_angle( glm::radians(angle) )
        , m_shoot_loop(loop)
        {/*NOP*/}   

        void initialize () override 
        {
            if (m_owner->hasComponent<TransformComponent>())
            {
                m_transform =  m_owner->getComponent<TransformComponent>();
                m_origin = m_transform->getPosition();
                m_transform->getVelocity().x =  glm::cos( m_angle) * m_speed;
                m_transform->getVelocity().y =  glm::sin( m_angle) * m_speed;
                //spdlog::info ("setting velocity {}, {}", m_transform->getVelocity().x, m_transform->getVelocity().y );
            }
        }
        
        void update (float /*deltaTime*/) override
        {
           if (m_transform == nullptr)
                return;
            if (glm::distance ( m_transform->getPosition(), m_origin)  > m_range )
            {
                if (m_shoot_loop)
                {
                   m_transform->getPosition().x =  m_origin.x;
                   m_transform->getPosition().y =  m_origin.y;
                }
                else
                {
                    m_owner->destroy();
                }
            }                  
        } 

        void render(  SDL_Renderer *  /*a_renderer*/ ) override
        {/*NOP*/}

        
};

