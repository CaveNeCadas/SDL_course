#include "texturemanager.hpp"


SDL_Texture* TextureManager::loadTexture (char const * filename, SDL_Renderer * a_renderer)
{
    SDL_Surface * surface = IMG_Load (filename);
    SDL_Texture * texture = SDL_CreateTextureFromSurface ( a_renderer,surface);
    SDL_FreeSurface (surface);
    return texture;

}

void TextureManager::draw ( SDL_Texture * texture , rect_t const &  src_rect, rect_t const &  dst_rect, SDL_RendererFlip flip ,  SDL_Renderer * a_renderer)
{
    #ifdef GAME_AVX
        SDL_Rect const r_1 {src_rect[0],src_rect[1],src_rect[2],src_rect[3] };
        SDL_Rect const r_2 {dst_rect[0],dst_rect[1],dst_rect[2],dst_rect[3] };
        SDL_RenderCopyEx ( a_renderer, texture, &r_1, &r_2, 0.0, NULL, flip );
    #else
        SDL_RenderCopyEx ( a_renderer, texture, &src_rect, &dst_rect, 0.0, NULL, flip );
    #endif

}
