#pragma once

#include <stdbool.h>
#include <SDL.h>

#include "player.h"

#define WINDOW_W 375
#define WINDOW_H 667

enum GameState {CR_STATE_WAITING, CR_STATE_STARTING, CR_STATE_STARTED};


typedef struct Game
{
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture *background;
    int mapWidth, mapHeight;
    Player player;
    enum GameState state;
} Game;

Game* GetGame();
