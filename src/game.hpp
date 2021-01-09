#ifndef _THE_GAME_HPP
#define _THE_GAME_HPP

#include <memory>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "glm/glm.hpp"

class Game
{
    public:

        explicit Game ();
        ~Game();
         void initialize(int32_t width, int32_t height);
         void processInput ();
         void update();
         void render();
         void destroy();       
         bool isRunning() const { return m_isrunning; }
    
    
    private:
        bool m_isrunning;
        SDL_Window* m_mainWindow;
        SDL_Renderer * m_renderer;

        glm::vec2 projectilePosition;
        glm::vec2 projectileVelocity;
        uint32_t ticklastframe;

        
};

using game_ptr = std::unique_ptr<Game>;

#endif