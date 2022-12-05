#include <iostream>
#include <memory>
#include "spdlog/spdlog.h"
#include "constants.hpp"
#include "game.hpp"

#if defined (_GAME_WIN32)
    #define entry_point wmain
#else
    #define entry_point main
#endif

int entry_point()
{
    spdlog::info ("Welcome!!!.");
    spdlog::info ("Width {}. Height {}", WINDOW_WIDTH, WINDOW_HEIGHT );

    game_ptr game = std::make_unique<Game>();

    game -> initialize (WINDOW_WIDTH, WINDOW_HEIGHT );

    game -> loadLevel(1);
    
    spdlog::info ("load level 1");

    while (game->isRunning())
    {
        game -> processInput();
        game -> update ();
        game -> render ();
    }
    return 0;
}