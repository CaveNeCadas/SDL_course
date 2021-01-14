
#include "fontnanager.hpp"

TTF_Font* FontManager::loadFont (char const * filename, int32_t size)
{
        return TTF_OpenFont (filename, size);
}

void FontManager::draw( SDL_Texture* atexture, SDL_Rect position, SDL_Renderer* arenderer)
{
    SDL_RenderCopy (arenderer, atexture, NULL, &position  );
}
