#ifndef FONT_MANAGER_OF_THE_GAME_HPP
#define FONT_MANAGER_OF_THE_GAME_HPP

#include "graphicshdr.hpp"

class FontManager
{
    public:    
        static TTF_Font* loadFont (char const * filename, int32_t size);
        static void draw( SDL_Texture* atexture, SDL_Rect position, SDL_Renderer* arenderer);
};


#endif