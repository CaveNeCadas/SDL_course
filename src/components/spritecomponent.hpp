#ifndef SPRITE_COMPONENT_OF_THE_GAME_HPP
#define SPRITE_COMPONENT_OF_THE_GAME_HPP

#include <string>
#include "graphicshdr.hpp"
#include "component.hpp"
#include "texturemanager.hpp"
#include "transformComponent.hpp"
#include "assetmanager.hpp"

class SpriteComponent : public Component
{
    private:
    TransformComponent* m_transformComponent;
        SDL_Texture * m_texture;
        SDL_Rect m_src_rect;
        SDL_Rect m_dst_rect;
        SDL_RendererFlip m_flip;

   public:

        SpriteComponent( Entity* owner, AssetManager*astmngr,  std::string const & texId )
        : Component(owner)
        , m_transformComponent{nullptr}
        , m_texture( nullptr )
        , m_src_rect{0,0,0,0}
        , m_dst_rect{0,0,0,0}
        , m_flip{SDL_FLIP_NONE}
        {/*NOP*/
            setTexture (astmngr, texId);        
        }   

        void setTexture ( AssetManager*astmngr,  std::string const & texId )
        {
                m_texture = astmngr->getTexture (texId);
        }

        void initialize () override 
        {
          //  spdlog::info ("initialize");
            m_transformComponent = m_owner->getComponent<TransformComponent>();
            m_src_rect.x = 0;
            m_src_rect.y = 0;
            m_src_rect.w = m_transformComponent->getWidth();
            m_src_rect.h = m_transformComponent->getHeight();
        }
        
        void update (float deltaTime) override
        {
           // spdlog::info ("update");
            m_dst_rect.x = m_transformComponent->getPosition().x;
            m_dst_rect.y = m_transformComponent->getPosition().y;
            m_dst_rect.w = m_transformComponent->getWidth() * m_transformComponent->getScale();
            m_dst_rect.h = m_transformComponent->getHeight()* m_transformComponent->getScale();        
        } 

        void render(  SDL_Renderer * a_renderer ) override
        {
            //spdlog::info ("draw");
            TextureManager::draw ( m_texture, m_src_rect, m_dst_rect, m_flip , a_renderer);
        }
};

#endif