#pragma once

#include <stdbool.h>
#include <SDL.h>

typedef struct
{
    SDL_Rect rect;
    SDL_Rect playerPosition;
    bool infected;
} Player;

void CreatePlayer(Player* player);
