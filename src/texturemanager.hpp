#ifndef TEXTURE_MANAGER_OF_THE_GAME_HPP
#define TEXTURE_MANAGER_OF_THE_GAME_HPP

#include "graphicshdr.hpp"

class TextureManager
{
    public:
        static SDL_Texture* loadTexture (char const * filename, SDL_Renderer * a_renderer);
        static void draw ( SDL_Texture * texture , SDL_Rect  src_rect, SDL_Rect  dst_rect, SDL_RendererFlip flip ,  SDL_Renderer * a_renderer);  
};


#endif