#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef struct
{
    SDL_Texture* textureMenu;
    SDL_Texture* texturePlayButton;
    SDL_Texture* textureExitButton;
    SDL_Rect rectPlayButton;
    int heigth,width;
    int x,y;
    bool menuClick;
}Menu;


void LoadMenu(Menu* menu);
void RenderMenu();
void HandleMenuEvents(SDL_Event* event);
void CheckMousePosition();