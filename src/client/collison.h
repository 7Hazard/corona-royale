#pragma once

#include <SDL.h>

void HandleBorders(SDL_Rect* rect, int frameHeight, int frameWidth);
bool DoBoxesIntersect(SDL_Rect* a, SDL_Rect* b);
