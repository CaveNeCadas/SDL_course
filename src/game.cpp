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

#ifdef GAME_AVX
    rect_t  Game::s_camera{ ._sides=_mm_set_epi32(0,0,WINDOW_WIDTH,WINDOW_HEIGHT) };
#else
    rect_t  Game::s_camera{0,0,WINDOW_WIDTH,WINDOW_HEIGHT};
#endif



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

    spdlog::info ("initialize...");

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
    spdlog::info ("SDL_CreateWindow...");
    // SDL_Window* SDL_CreateWindow(const char* title,
    //                              int         x,
    //                              int         y,
    //                              int         w,
    //                              int         h,
    //                              Uint32      flags)
    m_mainWindow = SDL_CreateWindow ( "my Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS );

   if ( nullptr == m_mainWindow )
    {
        spdlog::critical ("Cannot create window");
        return;
    }

    m_renderer = SDL_CreateRenderer (m_mainWindow, -1,0 );
    spdlog::info ("SDL_CreateRenderer...");

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
    sol::state lua_engine;
    lua_engine.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);
    std::string const level_to_load ( std::string("Level") + std::to_string(level) );
    std::string const file_to_load ( std::string("./assets/scripts/") + level_to_load + std::string(".lua") );

    lua_engine.script_file ( file_to_load );
    
    spdlog::info ("reading {} from file {}", level_to_load, file_to_load );

    sol::table levelData = lua_engine["Level1"];
    sol::table assetsTable = levelData["assets"]; 
    uint32_t assetIndex{0};
    while (true)
    {
        sol::optional<sol::table> exists_asset_node = assetsTable[assetIndex];
        if (exists_asset_node == sol::nullopt)
        {
            break;
        }
        std::string const asset_type( assetsTable[assetIndex]["type"] );
        if( asset_type == "texture" )
        {
            std::string const asset_id = assetsTable[assetIndex]["id"];
            std::string const asset_file = assetsTable[assetIndex]["file"];
           
            m_assetmanager->addTexture (hash(asset_id.c_str()), asset_file.c_str() );
        }
        else if( asset_type == "font" )
        {
            std::string const asset_id = assetsTable[assetIndex]["id"];
            std::string const asset_file = assetsTable[assetIndex]["file"];
            int32_t size = assetsTable[assetIndex]["fontSize"];
            m_assetmanager->addFont (hash(asset_id.c_str()), asset_file.c_str(), size );
        }
        else
        {
            spdlog::critical ("unknown aasset type {}", asset_type );
            return;
        }
        assetIndex += 1;
    }

    sol::table  mapTable = levelData["map"]; 
    std::string maptextureAssetId = mapTable["textureAssetId"];
    std::string mapfile = mapTable["file"];
    int32_t mapscale    = mapTable["scale"];
    int32_t maptileSize = mapTable["tileSize"];
    int32_t mapmapSizeX = mapTable["mapSizeX"];
    int32_t mapmapSizeY = mapTable["mapSizeY"];

    m_gameMap = std::make_unique<Map>( m_entityManager.get(), m_assetmanager->getTexture(hash(maptextureAssetId.c_str())), mapscale, maptileSize );
    m_gameMap->loadMap (mapfile.c_str(), mapmapSizeX, mapmapSizeY );


    sol::table  entities_Table = levelData["entities"]; 
    uint32_t entity_Index{0};

    while (true)
    {
        sol::optional<sol::table> exists_entity_node = entities_Table[entity_Index];
         if (exists_entity_node == sol::nullopt)
        {
            break;
        }

        auto entity_player_id = hash (   (std::string ( entities_Table[entity_Index]["name"] ) ).c_str()  );
        auto entity_layer = static_cast<LayerType>(entities_Table[entity_Index]["layer"] );

        Entity* current_entity = m_entityManager->addEntity ( entity_player_id, entity_layer );
        if (0 == entity_Index)
        {
            m_game_main_entity = current_entity;  
        } 

        sol::table component_table = entities_Table[entity_Index]["components"];
        sol::optional<sol::table> exists_entity_transformcomponent = component_table["transform"];

        int32_t transform_position_x{0};
        int32_t transform_position_y{0};
        int32_t transform_velocity_x{0};
        int32_t transform_velocity_y{0};
        int32_t transform_width{0};
        int32_t transform_height{0};

        if (exists_entity_node != sol::nullopt)
        {
            transform_position_x = component_table["transform"]["position"]["x"];
            transform_position_y = component_table["transform"]["position"]["y"];
            transform_velocity_x = component_table["transform"]["velocity"]["x"];
            transform_velocity_y = component_table["transform"]["velocity"]["y"];
            transform_width      = component_table["transform"]["width"];
            transform_height     = component_table["transform"]["height"];

            current_entity->addComponent<TransformComponent>( transform_position_x
                                                            , transform_position_y
                                                            , transform_velocity_x
                                                            , transform_velocity_y
                                                            , transform_width
                                                            , transform_height
                                                            , component_table["transform"]["scale"]
                                                            );
        }
        sol::optional<sol::table> exists_entity_spritecomponent = component_table["sprite"];
        if (exists_entity_spritecomponent != sol::nullopt)
        {
            bool is_animated = component_table["sprite"]["animated"];
            int32_t animationSpeed{1};
            int32_t frameCount{1};
            bool hasDirections{false};
            bool fixed{false};

            if (is_animated)
            {
                animationSpeed  = component_table["sprite"]["animationSpeed"];
                frameCount      = component_table["sprite"]["frameCount"];
                hasDirections   = component_table["sprite"]["hasDirections"];
                fixed           = component_table["sprite"]["fixed"];
            }

            current_entity->addComponent<SpriteComponent>( m_assetmanager->getTexture(hash((std::string(component_table["sprite"]["textureAssetId"])).c_str())) 
                                                         , animationSpeed
                                                         , frameCount
                                                         , hasDirections
                                                         , fixed
                                                         );
        }

        sol::optional<sol::table> exists_entity_collidercomponent = component_table["collider"];
        if (exists_entity_collidercomponent != sol::nullopt)
        {
            current_entity->addComponent<ColliderComponent>( component_table["collider"]["tag"], 240,120,32,32  );
        }

        sol::optional<sol::table> exists_entity_inputcomponent = component_table["input"];
        if (exists_entity_inputcomponent != sol::nullopt)
        {
            sol::optional<sol::table> exists_entity_inputkeyboardcomponent = component_table["input"]["keyboard"];
            if (exists_entity_inputkeyboardcomponent != sol::nullopt)
            {
                current_entity->addComponent<KeyboardComponent>( &m_event
                                                                , component_table["input"]["keyboard"]["up"]
                                                                , component_table["input"]["keyboard"]["right"]
                                                                , component_table["input"]["keyboard"]["down"]
                                                                , component_table["input"]["keyboard"]["left"]
                                                                , component_table["input"]["keyboard"]["shoot"]
                                                                );
            }
        }


        sol::optional<sol::table> exists_entity_projectileEmittercomponent = component_table["projectileEmitter"];
        if (exists_entity_projectileEmittercomponent != sol::nullopt)
        {
            int32_t speed   = component_table["projectileEmitter"]["speed"];
            int32_t range   = component_table["projectileEmitter"]["range"];
            float angle     = component_table["projectileEmitter"]["angle"];
            //bool shouldLoop = component_table["projectileEmitter"]["shouldLoop"].get_or(false);
            int32_t width   = component_table["projectileEmitter"]["width"].get_or(4);
            int32_t height  = component_table["projectileEmitter"]["height"].get_or(4);
            std::string textureAssetId = component_table["projectileEmitter"]["textureAssetId"].get_or(std::string("projectile-texture"));//hash();
            auto projectile_entity = m_entityManager->addEntity ( hash("enemy-projectile")  , LayerType::PROJECTILE_LAYER );
                projectile_entity->addComponent<TransformComponent> ( transform_position_x + (transform_width/2) 
                                                                    , transform_position_y + (transform_height/2) , 0,0, width,height,1);
                projectile_entity->addComponent<SpriteComponent>( m_assetmanager->getTexture(hash(textureAssetId.c_str()) ) );
                projectile_entity->addComponent<ColliderComponent>( "projectile tag", transform_position_x + (transform_width/2)  
                                                                  ,  transform_position_y + (transform_height/2) , width, height );
                projectile_entity->addComponent<ProjectileEmitterComponent>( speed, angle, range, true );
        }
        entity_Index += 1;
    }
    

    auto text_entity = m_entityManager->addEntity ( hash("levelIndicator"), LayerType::UI_LAYER);
        char title[80];
        sprintf(title, "Level %d", level);
        text_entity->addComponent<TextLabelComponent>( 10,10, title, m_assetmanager->getFont(hash("charriot-font")) , WHITE_COLOR, m_renderer  );

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
      if (nullptr != m_game_main_entity)
      {
        auto const tagcollided = m_entityManager->checkCollision(m_game_main_entity);
        if (tagcollided != 0)
        {
            spdlog::info("collision between between {}, {} ",  entity_player_id_t::value,  tagcollided );
            m_isrunning = false;
        }
      }
}

void Game::handleCameraMovement()
{
    if (m_entityManager && nullptr != m_game_main_entity)
    {
       #ifdef GAME_AVX
       #else
        auto transform = m_game_main_entity->getComponent<TransformComponent>();
        Game::s_camera.x = static_cast< decltype(Game::s_camera.x)  > ( transform->getPosition().x - (WINDOW_WIDTH / 2)  ) ;
        Game::s_camera.y = static_cast< decltype(Game::s_camera.x)  > ( transform->getPosition().y - (WINDOW_HEIGHT / 2) ); 

        Game::s_camera.x = Game::s_camera.x < 0 ? 0 : Game::s_camera.x;
        Game::s_camera.y = Game::s_camera.y < 0 ? 0 : Game::s_camera.y;
        
        Game::s_camera.x = Game::s_camera.x > Game::s_camera.w ? Game::s_camera.w : Game::s_camera.x;
        Game::s_camera.y = Game::s_camera.y > Game::s_camera.h ? Game::s_camera.h : Game::s_camera.y;

       #endif 
    }
}
