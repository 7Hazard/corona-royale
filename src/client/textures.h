#pragma once

#include <SDL_image.h>
#include "shared/hashtable.h"

typedef struct Textures
{
    SDL_Texture* grass;

    int playerWidth, playerHeight;
    SDL_Texture* healthyPlayer;
    SDL_Texture* infectedPlayer;
} Textures;

Textures* GetTextures();
