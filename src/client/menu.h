#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef struct
{
    SDL_Texture* textureMenu;
    SDL_Texture* texturePlayButton;
    SDL_Texture* textureExitButton;
    int heigth;
    int width;
    
}Menu;


void LoadMenu(Menu* menu);

void ShowMenu(Menu* menu);

void RenderMenu();