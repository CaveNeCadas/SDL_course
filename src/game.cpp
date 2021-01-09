#include "game.hpp"
#include "spdlog/spdlog.h"
#include "constants.hpp"

Game::Game ()
: m_isrunning{false}
, m_mainWindow{nullptr}
, m_renderer{nullptr}
, projectilePosition(0.f,0.0f)
, projectileVelocity(20.f, 20.f)
, ticklastframe(0)
{

}

Game::~Game()
{
    spdlog::info ("release resources");
    destroy();
    spdlog::info ("Bye");
}


void Game::initialize(int32_t width, int32_t height)
{
    if ( 0 != SDL_Init (SDL_INIT_EVERYTHING)  )
    {
        spdlog::critical ("Cannot initialize SDL");
        return;
    }   
    // SDL_Window* SDL_CreateWindow(const char* title,
    //                              int         x,
    //                              int         y,
    //                              int         w,
    //                              int         h,
    //                              Uint32      flags)
    m_mainWindow = SDL_CreateWindow ( "my Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS );

   if ( nullptr == m_mainWindow )
    {
        spdlog::critical ("Cannot create window");
        return;
    }

    m_renderer = SDL_CreateRenderer (m_mainWindow, -1,0 );

    if ( nullptr == m_renderer )
    {
        spdlog::critical ("Cannot create renderer");
        return;
    }

    m_isrunning = true;
}

void Game::processInput ()
{

    SDL_Event event;
    SDL_PollEvent (&event);

    switch ( event.type ) 
    {
    case SDL_QUIT:
        m_isrunning = false;
        break;
    
    case SDL_KEYDOWN:
        m_isrunning = !( event.key.keysym.sym == SDLK_ESCAPE)  ;
        break;
    default:
        break;
    }

}

void Game::update()
{
    //wait until 16ms has elapsed since last frame
    auto time2Wait =  FRAME_TARGET_TIME - (SDL_GetTicks() - ticklastframe );
    if (time2Wait > 0 && time2Wait <= FRAME_TARGET_TIME )
    {
        SDL_Delay (time2Wait);
    }
 
    //delta time is diff from last frame converted in seconds
    float deltaTime = ( SDL_GetTicks() - ticklastframe )/ 1000.f;

    ticklastframe =   SDL_GetTicks() ; // ticks since SDL_init;

    //clamp the value of delta time in case it's bigger than a max value
    
    deltaTime = ( deltaTime > 0.05f)? 0.05f: deltaTime;

    projectilePosition =  glm::vec2 ( projectilePosition.x + projectileVelocity.x  * deltaTime ,
                                      projectilePosition.y + projectileVelocity.y  * deltaTime 
                                     ); // how many pixel / second
    
}

void Game::render()
{
    SDL_SetRenderDrawColor ( m_renderer, 0,0,15, 255 );
    SDL_RenderClear(m_renderer);

    SDL_Rect projectile {  (int)projectilePosition.x, (int)projectilePosition.y, 10, 10 };

    SDL_SetRenderDrawColor ( m_renderer, 255,255,255, 255 );
    SDL_RenderFillRect ( m_renderer, &projectile );

    SDL_RenderPresent (m_renderer);
    
}

void Game::destroy()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow (m_mainWindow);
    SDL_Quit();
}