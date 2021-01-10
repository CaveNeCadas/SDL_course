#ifndef ASSET_MANAGER_OF_THE_GAME_HPP
#define ASSET_MANAGER_OF_THE_GAME_HPP

#include <map>
#include <string>
#include "graphicshdr.hpp"
#include "entitymanager.hpp"

class AssetManager
{
    private:
        Entitymanager* m_entitymanager;
        SDL_Renderer * m_renderer;
        std::map<std::string, SDL_Texture*> m_textures;
    public:
        AssetManager ( Entitymanager* entmngr, SDL_Renderer * a_renderer); 
        ~AssetManager();
        void clear();
        void addTexture (std::string textId, char const * filename);
        SDL_Texture* getTexture( std::string const & tid );

};


#endif