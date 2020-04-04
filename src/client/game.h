#pragma once

#include <stdbool.h>
#include <SDL.h>

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

typedef struct Game
{
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
} Game;

EXTERN Game* GetGame();
