#ifndef SPRITE_COMPONENT_OF_THE_GAME_HPP
#define SPRITE_COMPONENT_OF_THE_GAME_HPP

#include <string>
#include "graphicshdr.hpp"
#include "component.hpp"
#include "texturemanager.hpp"

class SpriteComponent : public Component
{
    private:
        SDL_Texture * m_texture;
        SDL_Rect m_src_rect;
        SDL_Rect m_dst_rect;
        SDL_RendererFlip m_flip;

   public:

        SpriteComponent( Entity* owner, char const * filepath )
        : Component(owner)
        , m_texture( nullptr )
        , m_src_rect{0,0,0,0}
        , m_dst_rect{0,0,0,0}
        , m_flip{SDL_FLIP_NONE}
        {/*NOP*/}   

        void setTexture (  std::string const & texId )

        void initialize () override 
        {/*NOP*/}
        
        void update (float deltaTime) override
        {                          
        } 

        void render(  SDL_Renderer * a_renderer ) override
        {
            SDL_Rect transformRect = { (int32_t)m_position.x, (int32_t)m_position.y, m_width, m_height  };
            SDL_SetRenderDrawColor ( a_renderer, 255,255,255, 255 );
            SDL_RenderFillRect ( a_renderer, &transformRect );
        }
};

#endif