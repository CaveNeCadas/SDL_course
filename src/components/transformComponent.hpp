#ifndef TRANSFORM_COMPONENT_OF_THE_GAME_HPP
#define TRANSFORM_COMPONENT_OF_THE_GAME_HPP

#include "graphicshdr.hpp"
#include "component.hpp"

class TransformComponent : public Component
{
    private:
        vec2_t m_position;
        vec2_t m_velocity;
         #ifdef GAME_AVX
            rect_t m_width_height;
         #else
            int32_t m_width;
            int32_t m_height;
         #endif
        int32_t m_scale;
    public:

        TransformComponent( Entity* owner, float pos_x, float pos_y, float vel_x, float vel_y, int32_t w, int32_t h, int32_t sca)
        : Component(owner)
        #ifdef GAME_AVX
            , m_position { ._coords={_mm_set_ps (pos_x, pos_y, 0.f, 0.f) } } 
            , m_velocity { ._coords={_mm_set_ps (vel_x, vel_y, 0.f, 0.f) } } 
            , m_width_height{ ._sides= _mm_set_epi32 (0, 0, w, h)  } 
        #else
            , m_position(pos_x, pos_y)
            , m_velocity(vel_x, vel_y)
            , m_width(w)
            , m_height(h)
        #endif
        , m_scale(sca)
        {/*NOP*/}   

        void initialize () override 
        {/*NOP*/}
        
        void update (float deltaTime) override
        {
            #ifdef GAME_AVX
                m_position  = _mm_add_ps( m_position,  _mm_mul_pd( m_velocity, _mm_set1_ps( deltaTime ) ) );  
            #else
                m_position = glm::vec2 ( m_position.x + m_velocity.x * deltaTime
                                    , m_position.y + m_velocity.y * deltaTime 
                                    );

            #endif
            
                                   
        } 

        void render(  SDL_Renderer * /*a_renderer*/ ) override
        {
           // SDL_Rect transformRect = { (int32_t)m_position.x, (int32_t)m_position.y, m_width, m_height  };
           // SDL_SetRenderDrawColor ( a_renderer, 0,0,16, 1 );
           // SDL_RenderFillRect ( a_renderer, &transformRect );
        }

        int32_t getWidth() const { return m_width;} 
        int32_t getHeight() const { return m_height;}
        int32_t getScale() const { return m_scale;} 
        #ifdef GAME_AVX
            rect_t getWidthHeight() const { return m_width_height;}
            __m128i getWidthHeightScaled() const { return _mm_mul_epi32 ( m_width_height._sides, _mm_set1_epi32( m_scale) ) ;}
        #endif
        vec2_t const &  getPosition() const {  return m_position; }
        vec2_t const &  getVelocity() const {  return m_velocity; }

        vec2_t &  getVelocity() {  return m_velocity; }
        vec2_t &  getPosition() {  return m_position; }
        
        void  updateVelocity( double x, double  y) 
        {  
            #ifdef GAME_AVX
                m_velocity._xy[0] = x;
                m_velocity._xy[1] = y;
            #else
                m_velocity.x = x; 
                m_velocity.y = y; 
            #endif                
        }
};

#endif