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
        rect_t m_src_rect;
        rect_t m_dst_rect;
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

         #ifdef GAME_AVX
            , m_src_rect{ ._side = { _mm_set1_epi32(0)}}
            , m_dst_rect{ ._side = { _mm_set1_epi32(0)}}
         #else
            , m_src_rect{0,0,0,0}
            , m_dst_rect{0,0,0,0}
         #endif           
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
            #ifdef GAME_AVX
                m_src_rect._sides = _mm_set_epi32( 0,0, m_transformComponent->getWidth(),m_transformComponent->getHeight() ) ;
            #else
                m_src_rect.x = 0;
                m_src_rect.y = 0;
                m_src_rect.w = m_transformComponent->getWidth();
                m_src_rect.h = m_transformComponent->getHeight();
            #endif
        }
        
        void update (float /*deltaTime*/) override
        {
            if (nullptr == m_transformComponent)
            {
                return;
            }
            if (m_isAnimated)
            {
                #ifdef GAME_AVX
                    m_src_rect._xywh[0] = m_src_rect._xywh[2] * static_cast<int>((SDL_GetTicks() / m_animation_speed) % m_num_frame);                
                #else
                    m_src_rect.x = m_src_rect.w * static_cast<int>((SDL_GetTicks() / m_animation_speed) % m_num_frame);                
                #endif
            }
      
            #ifdef GAME_AVX                
                m_src_rect._xywh[1] =  m_animation_index * m_transformComponent->getHeight();
                auto const vec_is_fixed = _mm_mul_epi32( Game::s_camera, _mm_set_epi32(static_cast<int32_t>(m_isfixed)));
                auto const vec_xy_cam = _mm_sub_epi32( _mm_castps_si128 (m_transformComponent->getPosition()), vec_is_fixed );
                auto const vec_wh_ = m_transformComponent->getWidthHeightScaled();
                m_dst_rect._sides = _mm_blend_epi32(vec_xy_cam,  vec_wh_, 0b1100);

            #else
                m_src_rect.y =  m_animation_index * m_transformComponent->getHeight();
                m_dst_rect.x = static_cast<int>( m_transformComponent->getPosition().x ) - (m_isfixed? 0 : Game::s_camera.x);
                m_dst_rect.y = static_cast<int>( m_transformComponent->getPosition().y ) - (m_isfixed? 0 : Game::s_camera.y);
                m_dst_rect.w = m_transformComponent->getWidth() * m_transformComponent->getScale();
                m_dst_rect.h = m_transformComponent->getHeight()* m_transformComponent->getScale();        
            #endif

      
        } 

        void render(  SDL_Renderer * a_renderer ) override
        {
            TextureManager::draw ( m_texture, m_src_rect, m_dst_rect, m_flip , a_renderer);
        }
};

#endif