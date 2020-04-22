#pragma once

#include <stdbool.h>
#include <SDL.h>

#include "player.h"

#define WINDOW_W 375
#define WINDOW_H 667

typedef struct Game
{
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture *background;
    int mapWidth, mapHeight;
    Mouse mouse;
    Player player;
} Game;

Game* GetGame();
