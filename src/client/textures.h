#pragma once

#include <SDL_image.h>
#include "shared/hashtable.h"

typedef struct Textures
{
    SDL_Texture* grass;

    SDL_Texture* healthyPlayer;
    SDL_Texture* infectedPlayer;
    int playerWidth, playerHeight;

    SDL_Texture* textureMenu;
    SDL_Texture* textureLogo;
    SDL_Texture* textureTextBox;
    SDL_Texture* textureGrayBox;
    SDL_Texture* textureTimerClock;

    SDL_Texture* arrowIndicator;
} Textures;

Textures* GetTextures();
void DisposeTextures();
