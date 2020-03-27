#pragma once

#include <stdbool.h>
#include <SDL.h>
#include <SDL_mixer.h>

extern int running;
extern SDL_Window* window;
extern Mix_Chunk *walking;
extern Mix_Chunk *cough;
extern SDL_Rect rect;
extern bool pressed_w;
extern bool pressed_s;
extern bool pressed_a;
extern bool pressed_d;