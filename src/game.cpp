#include "game.hpp"
#include "spdlog/spdlog.h"
#include "constants.hpp"
#include "components/transformComponent.hpp"
#include "components/spritecomponent.hpp"
#include "components/keyboardcontrol.hpp"
#include "components/colliderComponent.hpp"
#include "components/textlabelComponent.hpp"
#include "components/projectileemiterComponent.hpp"

using entity_player_id_t    = std::integral_constant<uint32_t, hash("main_player")>;
using entity_tank_id_t      = std::integral_constant<uint32_t, hash("tank_enemy")>;
using entity_radar_id_t     = std::integral_constant<uint32_t, hash("radar_game")>;

SDL_Rect  Game::s_camera{0,0,WINDOW_WIDTH,WINDOW_HEIGHT};

 Game::Game ()
: m_isrunning{false}
, m_mainWindow{nullptr}
, m_renderer{nullptr}
, projectilePosition(0.f,0.0f)
, projectileVelocity(20.f, 20.f)
, ticklastframe(0)
{/*NOP*/}

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
    if (0 != TTF_Init() )
    {
        spdlog::critical ("Cannot initialize TTF");
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

    m_entityManager = std::make_unique < Entitymanager> ( m_renderer );
    m_assetmanager  = std::make_unique < AssetManager > ( m_entityManager.get(), m_renderer );
    m_game_main_entity = nullptr;

    m_isrunning = true;
}

void Game::processInput ()
{   
    SDL_PollEvent (&m_event);

    switch ( m_event.type ) 
    {
    case SDL_QUIT:
        m_isrunning = false;
        break;
    
    case SDL_KEYDOWN:
        m_isrunning = !( m_event.key.keysym.sym == SDLK_ESCAPE)  ;
        break;
 
    default:
        break;
    }

}

void Game::update()
{
    auto newTick = SDL_GetTicks();
    //wait until 16ms has elapsed since last frame
    auto time2Wait =  FRAME_TARGET_TIME - (  newTick - ticklastframe );
    if (time2Wait > 0 && time2Wait <= FRAME_TARGET_TIME )
    {
        SDL_Delay (time2Wait);
    }
     
    //delta time is diff from last frame converted in seconds
    float deltaTime = ( newTick - ticklastframe )/ 1000.f;

    ticklastframe =   newTick ; // ticks since SDL_init;

    //clamp the value of delta time in case it's bigger than a max value
    
    deltaTime = ( deltaTime > 0.05f)? 0.05f: deltaTime;

    m_entityManager->update(deltaTime);
    handleCameraMovement();
    checkCollision();
}

void Game::loadLevel (int level)
{
    m_assetmanager->addTexture (hash("tank-image"), "./assets/images/tank-big-right.png" );
    m_assetmanager->addTexture (hash("chopper-image"), "./assets/images/chopper-spritesheet.png" );
    m_assetmanager->addTexture (hash("radar-image"), "./assets/images/radar.png"  );
    m_assetmanager->addTexture (hash("map-image"), "./assets/tilemaps/jungle.png" );
    m_assetmanager->addTexture (hash("heliport-image"), "./assets/images/heliport.png");
    m_assetmanager->addTexture (hash("projectile-image"), "./assets/images/bullet-enemy.png");
    m_assetmanager->addFont    (hash("charriot-font"), "./assets/fonts/charriot.ttf", 32);

    m_gameMap = std::make_unique<Map>( m_entityManager.get(), m_assetmanager->getTexture(hash("map-image")), 2, 32 );
    m_gameMap->loadMap ("./assets/tilemaps/jungle.map", 25, 20 );

    m_game_main_entity = m_entityManager->addEntity (entity_player_id_t::value, LayerType::PLAYER_LAYER );
        m_game_main_entity->addComponent<TransformComponent>( 240,120,0,0,32,32,1);
        m_game_main_entity->addComponent<SpriteComponent>( m_assetmanager->getTexture(hash("chopper-image")) , 90,2, true, false );
        m_game_main_entity->addComponent<KeyboardComponent>(  &m_event, "up", "right", "down", "left", "space");
        m_game_main_entity->addComponent<ColliderComponent>( "player tag", 240,120,32,32  );

    auto tank_entity = m_entityManager->addEntity ( entity_tank_id_t::value  , LayerType::ENEMY_LAYER );
        tank_entity->addComponent<TransformComponent>( 150,495,0,0,32,32,1);
        tank_entity->addComponent<SpriteComponent>( m_assetmanager->getTexture(hash("tank-image")) );
        tank_entity->addComponent<ColliderComponent>( "tank tag", 150,495,32,32  );

    auto projectile_entity = m_entityManager->addEntity ( hash("enemy-projectile")  , LayerType::PROJECTILE_LAYER );
        projectile_entity->addComponent<TransformComponent>( 150 + 16 ,495+16 ,0,0, 4,4,1);
        projectile_entity->addComponent<SpriteComponent>( m_assetmanager->getTexture(hash("projectile-image") ) );
        projectile_entity->addComponent<ColliderComponent>( "projectile tag", 150 + 16 ,495 +16 , 4, 4  );
        projectile_entity->addComponent<ProjectileEmitterComponent>( 150, 270.f, 300, true );

    auto heliport = m_entityManager->addEntity (hash("Heliport"), LayerType::OBSTACLE_LAYER);
        heliport->addComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
        heliport->addComponent<SpriteComponent>( m_assetmanager->getTexture(hash("heliport-image")) );
        heliport->addComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);

    auto radar_entity = m_entityManager->addEntity (entity_radar_id_t::value, LayerType::ENEMY_LAYER);
        radar_entity->addComponent<TransformComponent>( 720,14,0,0,64,64,1);
        radar_entity->addComponent<SpriteComponent>( m_assetmanager->getTexture(hash("radar-image")) , 60,8, true, true );

    auto text_entity = m_entityManager->addEntity ( hash("levelIndicator"), LayerType::UI_LAYER);
        text_entity->addComponent<TextLabelComponent>( 10,10, "Level I", m_assetmanager->getFont(hash("charriot-font")) , WHITE_COLOR, m_renderer  );

    m_entityManager->initialize();
}

void Game::render()
{
    SDL_SetRenderDrawColor ( m_renderer, 0,0,15, 255 );
    SDL_RenderClear(m_renderer);
    m_entityManager->render();
    SDL_RenderPresent (m_renderer);    
}

void Game::destroy()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow (m_mainWindow);
    SDL_Quit();
}

void Game::checkCollision()
{
    auto const tagcollided = m_entityManager->checkCollision(m_game_main_entity);
    if (tagcollided != 0)
    {
        spdlog::info("collision between between {}, {} ",  entity_player_id_t::value,  tagcollided );
        m_isrunning = false;
    }
}

void Game::handleCameraMovement()
{
    if (m_entityManager && nullptr != m_game_main_entity)
    {
        auto transform = m_game_main_entity->getComponent<TransformComponent>();
        Game::s_camera.x = transform->getPosition().x - (WINDOW_WIDTH / 2);
        Game::s_camera.y = transform->getPosition().y - (WINDOW_HEIGHT / 2); 

        Game::s_camera.x = Game::s_camera.x < 0 ? 0 : Game::s_camera.x;
        Game::s_camera.y = Game::s_camera.y < 0 ? 0 : Game::s_camera.y;
        
        Game::s_camera.x = Game::s_camera.x > Game::s_camera.w ? Game::s_camera.w : Game::s_camera.x;
        Game::s_camera.y = Game::s_camera.y > Game::s_camera.h ? Game::s_camera.h : Game::s_camera.y;
    }
}
