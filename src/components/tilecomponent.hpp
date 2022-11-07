#ifndef TILE_COMPONENT_OF_THE_GAME_HPP
#define TILE_COMPONENT_OF_THE_GAME_HPP

#include "graphicshdr.hpp"
#include "component.hpp"
#include "texturemanager.hpp"
#include "game.hpp"

class TileComponent : public Component
{
    private:

        SDL_Texture* m_texture;
        rect_t m_src_rect;
        rect_t m_dst_rect;
        vec2_t m_position;

    public:

        TileComponent(Entity* owner, SDL_Texture* texture,  int32_t srcx, int32_t srcy, int32_t x, int32_t y, int32_t tilesize, int32_t tilescale)
        : Component(owner)
        , m_texture(texture)
         #ifdef GAME_AVX
            , m_src_rect{ ._sides = { _mm_set_epi32( srcx, srcy, tilesize, tilesize ) } }
            , m_dst_rect{ ._sides = { _mm_set_epi32( x, y, tilesize*tilescale, tilesize*tilescale)}}
            , m_position{ ._coords = { _mm_set_ps( static_cast<float> (x) , static_cast<float> (y), 0.f, 0.f)}}
         #else
            , m_src_rect{ srcx, srcy, tilesize, tilesize}
            , m_dst_rect{ x, y, tilesize*tilescale, tilesize*tilescale}
            , m_position(x,y)
         #endif
        
        {/*NOP*/}

        void initialize () override 
        {/*NOP*/}
        
        void update (float /*deltaTime*/) override
        {
            #ifdef GAME_AVX
                m_dst_rect._sides  = _mm_sub_epi32 ( mm_castps_si128(m_position._coords), Game::s_camera ) ;
            #else
                m_dst_rect.x = static_cast<decltype(SDL_Rect::x)> (m_position.x - Game::s_camera.x);
                m_dst_rect.y = static_cast<decltype(SDL_Rect::y)> (m_position.y - Game::s_camera.y);

            #endif
        } 

        void render(  SDL_Renderer * a_renderer ) override
        {
                TextureManager::draw ( m_texture, m_src_rect, m_dst_rect, SDL_FLIP_NONE, a_renderer);  
        }
};

#endif