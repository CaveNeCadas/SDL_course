#ifndef ASSET_MANAGER_OF_THE_GAME_HPP
#define ASSET_MANAGER_OF_THE_GAME_HPP

#include <map>
#include <unordered_map>
#include <string>
#include "graphicshdr.hpp"
#include "entitymanager.hpp"

class AssetManager
{
    private:
        Entitymanager* m_entitymanager;
        SDL_Renderer * m_renderer;
        
        std::unordered_map<uint32_t, SDL_Texture*> m_textures;
        std::unordered_map<uint32_t, TTF_Font*> m_fonts;
    public:
        AssetManager ( Entitymanager* entmngr, SDL_Renderer * a_renderer); 
        ~AssetManager();
        void clear();
        void addTexture ( uint32_t textId, char const * filename);
        SDL_Texture* getTexture( uint32_t tid );

        void addFont ( uint32_t fontId, char const * filename, int32_t size);
        TTF_Font* getFont( uint32_t fid );
};


#endif