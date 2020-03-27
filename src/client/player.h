#pragma once

#include <stdbool.h>
#include <SDL.h>
#include "main.h";
#include "events.h";


typedef struct
{
    SDL_Rect rect;
    bool infected;
    Mix_Chunk *walking;
    Mix_Music *steps;
} Player;

void CreatePlayer(Player* player);
void HandlePlayerEvents(SDL_Event *event);