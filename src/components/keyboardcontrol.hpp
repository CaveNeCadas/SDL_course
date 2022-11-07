#ifndef KEYBOARD_COMPONENT_OF_THE_GAME_HPP
#define KEYBOARD_COMPONENT_OF_THE_GAME_HPP

#include <string>

#include "graphicshdr.hpp"
#include "component.hpp"
#include "entity.hpp"
#include "components/spritecomponent.hpp"
#include "components/transformComponent.hpp"

class KeyboardComponent : public Component
{
    private:

        TransformComponent* transform{nullptr};
        SpriteComponent* sprite{nullptr};
        SDL_Event* m_event{nullptr};

        std::string m_upKey;
        std::string m_downKey;
        std::string m_leftKey;
        std::string m_rightKey;
        std::string m_shootKey;

    public:

        KeyboardComponent( Entity* owner, 
                            SDL_Event* event,
                            std::string upKey,
                            std::string rightKey,
                            std::string downKey,
                            std::string leftKey,
                            std::string shootKey
                         )
        : Component(owner)
        , m_event(event)
        , m_upKey   (KeyboardComponent::getSDLKeyCodes ( upKey ))
        , m_downKey (KeyboardComponent::getSDLKeyCodes (downKey))
        , m_leftKey (KeyboardComponent::getSDLKeyCodes (leftKey))
        , m_rightKey(KeyboardComponent::getSDLKeyCodes (rightKey))
        , m_shootKey(KeyboardComponent::getSDLKeyCodes (shootKey))
        {/*NOP*/}   

        void initialize () override 
        {
            transform = m_owner->getComponent<TransformComponent>();
            sprite = m_owner->getComponent<SpriteComponent>();
        }
        
         void update(float /*deltaTime*/) override 
         {
            if ( m_event->type == SDL_KEYDOWN) 
            {
                std::string keyCode = std::to_string( m_event->key.keysym.sym);

                if (keyCode.compare(m_upKey) == 0) 
                {
                    transform->updateVelocity ( 0, -25);
                    sprite->play("animation_up"); 
                }
                if (keyCode.compare(m_rightKey) == 0) 
                {
                    transform->updateVelocity ( 25, 0);
                    sprite->play("animation_right"); 
                }
                if (keyCode.compare(m_downKey) == 0) 
                {
                    transform->updateVelocity ( 0, 25);
                    sprite->play("animation_down"); 
                }
                if (keyCode.compare(m_leftKey) == 0) 
                {
                    transform->updateVelocity ( -25, 0);
                    sprite->play("animation_left"); 
                }
                if (keyCode.compare(m_shootKey) == 0) {
                    // TODO:
                    // Shoot projectiles when 'shoot' key is pressed                    
                }
            }

            if ( m_event->type == SDL_KEYUP) {
                std::string keyCode = std::to_string( m_event->key.keysym.sym);
                if (keyCode.compare(m_upKey) == 0) 
                {
                    transform->updateVelocity ( 0, 0);
                }
                if (keyCode.compare(m_rightKey) == 0) {
                   transform->updateVelocity ( 0, 0);
                }
                if (keyCode.compare(m_downKey) == 0) {
                    transform->updateVelocity ( 0, 0);
                }
                if (keyCode.compare(m_leftKey) == 0) {
                    transform->updateVelocity ( 0, 0);
                }
            }           
        }

        void render(  SDL_Renderer * /*a_renderer*/ ) override
        {
            
        }

   public:

        static std::string getSDLKeyCodes (std::string const & key)
        {
            if (key.compare("up") == 0) return SDL_GetKeyName (SDLK_w);
            if (key.compare("down") == 0) return SDL_GetKeyName (SDLK_s);
            if (key.compare("left") == 0) return SDL_GetKeyName (SDLK_a);
            if (key.compare("right") == 0) return SDL_GetKeyName (SDLK_d);
            if (key.compare("space") == 0) return SDL_GetKeyName (SDLK_SPACE); 
            return std::to_string(static_cast<int>(key[0]));

        }     

};

#endif