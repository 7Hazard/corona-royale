#pragma once

#include <stdbool.h>
#include <SDL.h>

#include "events.h"

typedef struct
{
    SDL_Rect rect;
    bool infected;
} Player;

void CreatePlayer(Player* player);
void HandlePlayerEvents(SDL_Event *event);
void OnPlayerUpdate(Player* player);
void OnPlayerRender(Player* player);
