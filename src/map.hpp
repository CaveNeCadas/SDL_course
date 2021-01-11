#ifndef MAP_OF_THE_GAME_HPP
#define MAP_OF_THE_GAME_HPP

#include <map>
#include <string>

#include "graphicshdr.hpp"
#include "entitymanager.hpp"

class Map
{
    private:

        SDL_Texture* m_texture;
        int m_scale;
        int m_tile_size;

        Entitymanager* m_entity_manangr;

    public:
        
        Map (Entitymanager* mangr, SDL_Texture* texture, int32_t scale, int32_t tilesisze);
        ~Map();
        void addTile (int32_t srcx, int32_t srcy, int32_t x, int32_t y);
        void loadMap (std::string const & filepath, int32_t sizex, int32_t sizey);
};


#endif