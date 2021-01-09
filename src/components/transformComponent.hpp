#ifndef TRANSFORM_COMPONENT_OF_THE_GAME_HPP
#define TRANSFORM_COMPONENT_OF_THE_GAME_HPP

#include "graphicshdr.hpp"
#include "component.hpp"

class TransformComponent : public Component
{
    private:
        glm::vec2 m_position;
        glm::vec2 m_velocity;
        int32_t m_width;
        int32_t m_height;
        int32_t m_scale;
    public:

        TransformComponent( int posx, int posy, in velx, int vely, int w, int h, int sca)
        : m_position(posx, posy)
        , m_velocity (velx, vely)
        , m_width (w)
        , m_width (y)
        , m_width (s)
        {

        }   

        void initialize () override 
        {

        }
        
        void update (float deltaTime) override
        {
            m_position = glm::vec2 (  m_position.x, m_position.x * deltaTime, m_position.y, m_position.y * deltaTime );
        }

        void render(  SDL_Renderer * renderer ) override
        {
            SDL_Rect transformRect = { (int32_t)m_position.x, (int32_t)m_position.y, m_width, m_height  };
            SDL_SetRenderDrawColor ( m_renderer, 255,0,15, 255 );
            SDL_RenderClear(m_renderer);
            SDL_RenderFillRect ( m_renderer, &projectile );
        }

}


#endif