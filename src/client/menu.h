#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>


typedef struct Menu
{
    SDL_Texture* textureMenu;
    SDL_Texture* textureLogo;
    SDL_Texture* textureTextBox;
    SDL_Rect logoRect;
    bool textBoxClick;

    SDL_Rect textBoxRect;
    SDL_Rect serverTextRect;
    SDL_Color boxBackgroundcolor;
    SDL_Color boxOutlineColor;
    SDL_Color textColor;
    char textInTextBox[50];
    int textLength,mouseX,mouseY;
} Menu;


void LoadMenu(Menu* menu);
void RenderMenu();
void HandleMenuEvents(SDL_Event* event);