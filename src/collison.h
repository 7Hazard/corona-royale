#pragma once

#include <SDL.h>

void HandleBorders(SDL_Rect* rect);
bool DoBoxesIntersect(SDL_Rect* a, SDL_Rect* b);
