#ifndef CONSTANTS_OF_THE_GAME_HPP
#define CONSTANTS_OF_THE_GAME_HPP

#include <cstdint>
#include <iostream>
#include <cassert>


const int32_t WINDOW_WIDTH = 800;
const int32_t WINDOW_HEIGHT = 800;

const uint32_t FPS = 60;

const uint32_t FRAME_TARGET_TIME = 1000 / FPS;

enum class LayerType : uint8_t
{ 
    DEFAULT_LAYER = 0,
    TILEMAP_LAYER = 1,
    VEGETATION_LAYER = 2,
    ENEMY_LAYER = 3,
    PLAYER_LAYER = 4,
    PROJECTILE_LAYER = 5
};




namespace hash_impl
{
	template <typename S> struct fnv_internal;
	template <typename S> struct fnv1a;

	template <> struct fnv_internal<uint32_t>
	{
		constexpr static uint32_t default_offset_basis = 0x811C9DC5;
		constexpr static uint32_t prime                = 0x01000193;
	};


	template <> struct fnv1a<uint32_t> : public fnv_internal<uint32_t>
	{
		constexpr static inline uint32_t hash(char const* const aString, const uint32_t val = default_offset_basis)
		{
			return (aString[0] == '\0') ? val : hash( &aString[1], ( val ^ uint32_t(aString[0]) ) * prime);
		}
	};
} // namespace hash


constexpr uint32_t inline hash( char const* const aString )
{
	return hash_impl::fnv1a<uint32_t>::hash(aString);
}


static_assert( hash("FNV Hash Test") == 0xF38B3DB9, "fnv1a_32::hash failure" );

#endif