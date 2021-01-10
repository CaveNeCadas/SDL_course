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

        TransformComponent( Entity* owner, int posx, int posy, int velx, int vely, int w, int h, int sca)
        : Component(owner)
        , m_position(posx, posy)
        , m_velocity(velx, vely)
        , m_width(w)
        , m_height(h)
        , m_scale(sca)
        {/*NOP*/}   

        void initialize () override 
        {/*NOP*/}
        
        void update (float deltaTime) override
        {
            m_position = glm::vec2 ( m_position.x + m_velocity.x * deltaTime
                                   , m_position.y + m_velocity.y * deltaTime 
                                   );
                                   
        } 

        void render(  SDL_Renderer * a_renderer ) override
        {
            SDL_Rect transformRect = { (int32_t)m_position.x, (int32_t)m_position.y, m_width, m_height  };
            SDL_SetRenderDrawColor ( a_renderer, 255,255,255, 255 );
            SDL_RenderFillRect ( a_renderer, &transformRect );
        }
};

#endif