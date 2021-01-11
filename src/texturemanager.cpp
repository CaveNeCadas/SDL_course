#include "texturemanager.hpp"


SDL_Texture* TextureManager::loadTexture (char const * filename, SDL_Renderer * a_renderer)
{
    SDL_Surface * surface = IMG_Load (filename);
    SDL_Texture * texture = SDL_CreateTextureFromSurface ( a_renderer,surface);
    SDL_FreeSurface (surface);
    return texture;

}

void TextureManager::draw ( SDL_Texture * texture , SDL_Rect  src_rect, SDL_Rect  dst_rect, SDL_RendererFlip flip ,  SDL_Renderer * a_renderer)
{
    SDL_RenderCopyEx ( a_renderer, texture, &src_rect, &dst_rect, 0.0, NULL, flip );
}
