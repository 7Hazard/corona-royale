#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef struct
{
    SDL_Texture* textureMenu;
    SDL_Texture* texturePlayButton;
    SDL_Texture* surfaceExitButton;
}Menu;


void LoadMenu(Menu* menu);