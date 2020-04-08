#pragma once

#include <SDL.h>
#include "player.h"

void HandleBorders(Player *player);
bool DoBoxesIntersect(SDL_Rect* a, SDL_Rect* b);
