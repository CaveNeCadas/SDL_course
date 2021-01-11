#include "assetmanager.hpp"
#include "texturemanager.hpp"
#include "spdlog/spdlog.h"


AssetManager::AssetManager ( Entitymanager* entmngr, SDL_Renderer * a_renderer)
: m_entitymanager(entmngr)
, m_renderer (a_renderer)
{/*nop*/}

AssetManager::~AssetManager()
{
    
}

void AssetManager::clear()
{
    m_textures.clear();
}

void AssetManager::addTexture (std::string textId, char const * filename)
{
    m_textures[textId] = TextureManager::loadTexture(filename, m_renderer);
}

SDL_Texture* AssetManager::getTexture(std::string const & tid) 
{   
    return m_textures[tid];
}