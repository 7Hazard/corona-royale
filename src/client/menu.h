#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>


typedef struct
{
    SDL_Texture* textureMenu;
    SDL_Texture* textureLogo;
    SDL_Rect logoRect;
    bool menuClick;
}Menu;


void LoadMenu(Menu* menu);
void RenderMenu();
void HandleMenuEvents(SDL_Event* event);