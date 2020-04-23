#pragma once

#include <stdbool.h>
#include <SDL.h>

#include "player.h"
#include "timer.h"
#include "menu.h"

#define WINDOW_W 375
#define WINDOW_H 667

typedef struct Game
{
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture *background;
    int mapWidth, mapHeight;
    Player player;
    Timer timer;
    Menu menu;
} Game;

enum States {CR_STATE_PRELOAD,CR_STATE_MENU,CR_STATE_LOADGAME,CR_STATE_RUNNING,CR_STATE_WINSCREEN}; 

Game* GetGame();
