#ifndef SPRITE_COMPONENT_OF_THE_GAME_HPP
#define SPRITE_COMPONENT_OF_THE_GAME_HPP

#include <string>
#include "graphicshdr.hpp"
#include "component.hpp"
#include "texturemanager.hpp"
#include "transformComponent.hpp"
#include "assetmanager.hpp"
#include "animation.hpp"
#include "game.hpp"

class SpriteComponent : public Component
{
    private:
    TransformComponent* m_transformComponent;
        SDL_Texture * m_texture;
        SDL_Rect m_src_rect;
        SDL_Rect m_dst_rect;
        SDL_RendererFlip m_flip;

        bool m_isAnimated;
        int32_t m_num_frame;
        int32_t m_animation_speed;
        bool m_isfixed;

        std::map <std::string, Animation> m_animations;
        std::string m_animation_name;
        int32_t m_animation_index;


   public:

        SpriteComponent( Entity* owner, SDL_Texture * texture )
        : SpriteComponent( owner, texture, 0,0, false, false )
        {/*NOP*/}   

        SpriteComponent( Entity* owner, SDL_Texture * texture, int32_t speed, int32_t numframe, bool hasdir, bool isfixed )
        : Component(owner)
        , m_transformComponent{nullptr}
        , m_texture( texture )
        , m_src_rect{0,0,0,0}
        , m_dst_rect{0,0,0,0}
        , m_flip{SDL_FLIP_NONE}
        , m_isAnimated{ speed > 0}
        , m_num_frame{ numframe }
        , m_animation_speed{speed}
        , m_isfixed{isfixed}
        , m_animation_name{ (hasdir ? "" : "single_animation") }
        , m_animation_index{ 0 }
        {
           
            if ( !hasdir )
            {
                m_animations [m_animation_name] = Animation (0,m_num_frame, m_animation_speed);
            }
            else
            {
                m_animations ["animation_right"] = Animation (1,m_num_frame, m_animation_speed);
                m_animations ["animation_left"] = Animation (2,m_num_frame, m_animation_speed);
                m_animations ["animation_up"] = Animation (3,m_num_frame, m_animation_speed);
                m_animations ["animation_down"] = Animation (0,m_num_frame, m_animation_speed);
                m_animation_name = "animation_down";
            }
        }

        void play( std::string const & anim_name )
        {
            m_num_frame       = m_animations[anim_name].getNumFrame();
            m_animation_index = m_animations[anim_name].getIndex();
            m_animation_speed = m_animations[anim_name].getSpeed();
            m_animation_name = anim_name;
        }

        void initialize () override 
        {
            if (!m_owner->hasComponent<TransformComponent>())
            {
                return;
            }
            m_transformComponent = m_owner->getComponent<TransformComponent>();
            m_src_rect.x = 0;
            m_src_rect.y = 0;
            m_src_rect.w = m_transformComponent->getWidth();
            m_src_rect.h = m_transformComponent->getHeight();
        }
        
        void update (float /*deltaTime*/) override
        {
            if (nullptr == m_transformComponent)
            {
                return;
            }
            if (m_isAnimated)
            {
                m_src_rect.x = m_src_rect.w * static_cast<int>((SDL_GetTicks() / m_animation_speed) % m_num_frame);                
            }
            m_src_rect.y =  m_animation_index * m_transformComponent->getHeight();
            m_dst_rect.x = static_cast<int>( m_transformComponent->getPosition().x ) - (m_isfixed? 0 : Game::s_camera.x);
            m_dst_rect.y = static_cast<int>( m_transformComponent->getPosition().y ) - (m_isfixed? 0 : Game::s_camera.y);
            m_dst_rect.w = m_transformComponent->getWidth() * m_transformComponent->getScale();
            m_dst_rect.h = m_transformComponent->getHeight()* m_transformComponent->getScale();        
        } 

        void render(  SDL_Renderer * a_renderer ) override
        {
            TextureManager::draw ( m_texture, m_src_rect, m_dst_rect, m_flip , a_renderer);
        }
};

#endif