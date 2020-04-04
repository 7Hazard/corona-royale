#pragma once

#include "game.h"

inline Game InitGame()
{
    Game game;

    if(SDL_Init(SDL_INIT_AUDIO)!=0) 
    {
        SDL_Log("SDL_Init: %s\n", SDL_GetError());
        abort();
        exit(0);
    }

    // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
    game.running = true;
    game.window = SDL_CreateWindow("Corona Royale",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,800,0);
    game.renderer = SDL_CreateRenderer(game.window, -1, 0);
    // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT

    return game;
}

// extern "C" required to not mangle function name
EXTERN Game* GetGame()
{
    // Will initialize after first call
    static Game game = InitGame(); // only works in C++
    return &game;
}
