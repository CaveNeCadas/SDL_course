#ifndef _THE_GAME_HPP
#define _THE_GAME_HPP

#include <memory>
#include "graphicshdr.hpp"

#include "entitymanager.hpp"


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

         void loadLevel (int level);
         bool isRunning() const { return m_isrunning; }
    
    
    private:
        bool m_isrunning;
        SDL_Window* m_mainWindow;
        SDL_Renderer * m_renderer;

        glm::vec2 projectilePosition;
        glm::vec2 projectileVelocity;
        uint32_t ticklastframe;

        std::unique_ptr <Entitymanager> m_entityManager; 
        
};

using game_ptr = std::unique_ptr<Game>;

#endif