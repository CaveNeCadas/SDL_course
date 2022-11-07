#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "glm/glm.hpp"
#include "spdlog/spdlog.h"


#ifdef GAME_AVX
    #include <immintrin.h>
    union rect_t
    {
        __m128i _sides;
        int32_t _xywh[4];
    };

    union vec2_t
    {
        __m128 _coords;
        float _xy[4];
    };

#else
    using rect_t = SDL_Rect ;   
    using vec2_t = glm::vec2;
#endif
