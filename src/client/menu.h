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
    bool cursorBlink;

    SDL_Rect textCursorRect;
    SDL_Rect textBoxRect;
    SDL_Rect serverTextRect;
    SDL_Color boxBackgroundcolor;
    SDL_Color boxOutlineColor;
    SDL_Color textColor;
    SDL_Color textCursorColor;
    char textInTextBox[50];
    int textLength,mouseX,mouseY,delay;
} Menu;


void LoadMenu(Menu* menu);
void RenderMenu();
void HandleMenuEvents(SDL_Event* event);