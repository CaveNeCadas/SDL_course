#ifndef TEXT_LABEL_COMPONENT_OF_THE_GAME_HPP
#define TEXT_LABEL_COMPONENT_OF_THE_GAME_HPP

#include "graphicshdr.hpp"
#include "component.hpp"
#include "fontmanager.hpp"

class TextLabelComponent : public Component
{
    private:
        SDL_Rect m_position;
        std::string m_text;
        //TTF_Font* m_fontId{nullptr};
        SDL_Color m_color;
        SDL_Texture* m_texture;
        SDL_Renderer* m_renderer;
    public:

        TextLabelComponent( Entity* owner, int x, int y, std::string text,TTF_Font* family, SDL_Color color, SDL_Renderer* arenderer)
        : Component(owner)
        , m_position {x, y,0,0}
        , m_text(std::move(text))
        , m_color(color)
        , m_texture{nullptr}
        , m_renderer{arenderer}
        {

              setLabelText ( m_text, family , arenderer);
        }   

        void initialize () override 
        {/*NOP*/}
        
        void update (float /*deltaTime*/) override
        {/*NOP*/} 

        void render(  SDL_Renderer * /*a_renderer*/ ) override
        {
                FontManager::draw( m_texture, m_position, m_renderer);
        }


        void setLabelText ( std::string text, TTF_Font* font , SDL_Renderer* arenderer)
        {
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), m_color );
            m_texture = SDL_CreateTextureFromSurface ( arenderer, surface );
            SDL_FreeSurface (surface);
            SDL_QueryTexture (m_texture, NULL, NULL, &m_position.w,&m_position.h );
        }
};

#endif