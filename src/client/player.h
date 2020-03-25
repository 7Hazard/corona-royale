#pragma once

#include <stdbool.h>
#include <SDL.h>

typedef struct
{
    SDL_Rect rect;
    bool infected;
} Player;

void CreatePlayer(Player* player);
