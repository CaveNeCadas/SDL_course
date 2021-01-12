#ifndef CONSTANTS_OF_THE_GAME_HPP
#define CONSTANTS_OF_THE_GAME_HPP

#include <cstdint>


const int32_t WINDOW_WIDTH = 800;
const int32_t WINDOW_HEIGHT = 800;

const uint32_t FPS = 60;

const uint32_t FRAME_TARGET_TIME = 1000 / FPS;

enum class LayerType : uint8_t
{ 
    DEFAULT_LAYER = 0,
    TILEMAP_LAYER = 1,
    VEGETATION_LAYER = 2,
    ENEMY_LAYER = 3,
    PLAYER_LAYER = 4,
    PROJECTILE_LAYER = 5
};

#endif