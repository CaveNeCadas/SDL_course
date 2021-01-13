#ifndef _THE_GAME_HPP
#define _THE_GAME_HPP

#include <memory>
#include "graphicshdr.hpp"

#include "entitymanager.hpp"
#include "assetmanager.hpp"
#include "map.hpp"
#include "entity.hpp"



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

        void handleCameraMovement();
        void checkCollision();
    
    private:
        
	    bool m_isrunning;
        SDL_Window* m_mainWindow;
        SDL_Renderer * m_renderer;
        SDL_Event m_event;

        glm::vec2 projectilePosition;
        glm::vec2 projectileVelocity;
        uint32_t ticklastframe;

        std::unique_ptr < Entitymanager > m_entityManager; 
        std::unique_ptr < AssetManager  > m_assetmanager;
        std::unique_ptr <Map>  m_gameMap;

        Entity* m_game_main_entity;


    public:
    
	static SDL_Rect s_camera;
};

using game_ptr = std::unique_ptr<Game>;

#endif
