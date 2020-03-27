#include <stdio.h>
#include <SDL.h>


#include "main.h"

void HandleBorders(SDL_Rect* rect)
{
    if (rect->x < 0 && rect->y < 0)
    {
        rect->x = 0;
        rect->y = 0;
    }
    else if (rect->x > 420 && rect->y < 0)
    {
        rect->x = 420;
        rect->y = 0;
    }
    else if (rect->x < 0 && rect->y > 720)
    {
        rect->x = 0;
        rect->y = 720;
    }
    else if (rect->x > 420 && rect->y > 720)
    {
        rect->x = 420;
        rect->y = 720;
    }
    else if (rect->x < 0)
    {
        rect->x = 0;
    }
    else if (rect->x > 420)
    {
        rect->x = 420;
    }
    else if (rect->y < 0)
    {
        rect->y = 0;
    }
    else if (rect->y > 720)
    {
        rect->y = 720;
    }
}

bool DoBoxesIntersect(SDL_Rect* a, SDL_Rect* b) {
  return (abs(a->x - b->x) * 2 < (a->w + b->w)) &&
         (abs(a->y - b->y) * 2 < (a->h + b->h));
}


