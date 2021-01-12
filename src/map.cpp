#include <fstream>
#include "map.hpp"
#include "entitymanager.hpp"
#include "components/tilecomponent.hpp"


Map::Map (Entitymanager* mangr,  SDL_Texture* texture, int32_t scale, int32_t tilesisze)
: m_texture(texture)
, m_scale(scale)
, m_tile_size(tilesisze)
, m_entity_manangr(mangr)
{

}

Map::~Map()
{
    
}

void Map::loadMap( std::string const & filepath, int32_t sizex, int32_t sizey)
{
    // read the map file defintion from the .map file
    std::fstream filetile;
    filetile.open (filepath.c_str());

    for (auto y = 0; y < sizey; ++y)
    {
        for (auto x = 0; x < sizex; ++x)
        {
            char ch;
            filetile.get(ch);
            int32_t srcy = std::atoi (&ch) * m_tile_size;
            filetile.get(ch);
            int32_t srcx = std::atoi (&ch) * m_tile_size;       

           //    
            addTile ( srcx,  srcy, x*(m_scale*m_tile_size), y*(m_scale*m_tile_size));
           // spdlog::info ("{}, {}, {}, {}", srcx, srcy, x*(m_scale*m_tile_size), y*(m_scale*m_tile_size) );  
            filetile.ignore();
        }
    }

    filetile.close();

}

void Map::addTile ( int32_t srcx, int32_t srcy, int32_t x, int32_t y)
{
    Entity* newTile =  m_entity_manangr->addEntity("Tile", LayerType::VEGETATION_LAYER);
    newTile->addComponent<TileComponent> ( m_texture, srcx,  srcy,  x,  y, m_tile_size, m_scale);

}
