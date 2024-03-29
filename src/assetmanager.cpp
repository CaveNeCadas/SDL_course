#include "assetmanager.hpp"
#include "texturemanager.hpp"
#include "fontmanager.hpp"
#include "spdlog/spdlog.h"


AssetManager::AssetManager ( Entitymanager* entmngr, SDL_Renderer * a_renderer)
: m_entitymanager(entmngr)
, m_renderer (a_renderer)
{/*nop*/}

AssetManager::~AssetManager()
{/*NOP*/}

void AssetManager::clear()
{
    m_textures.clear();
    m_fonts.clear();
}

void AssetManager::addTexture ( uint32_t textId, char const * filename)
{
    m_textures[textId] = TextureManager::loadTexture(filename, m_renderer);
}

SDL_Texture* AssetManager::getTexture( uint32_t tid) 
{   
    return m_textures[tid];
}

void AssetManager::addFont ( uint32_t fontId, char const * filename, int32_t size)
{
    m_fonts[fontId] = FontManager::loadFont(filename, size);
}

TTF_Font* AssetManager::getFont( uint32_t fid )
{
    return m_fonts[fid];
}