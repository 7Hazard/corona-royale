#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "main.h"

void HandleBorders(SDL_Rect* rect, int frameHeight, int frameWidth)
{
    if (rect->x < 0 && rect->y < 0)
    {
        rect->x = 0;
        rect->y = 0;
    }
    else if (rect->x > (WINDOW_W-frameWidth) && rect->y < 0)
    {
        rect->x = (WINDOW_W-frameWidth);
        rect->y = 0;
    }
    else if (rect->x < 0 && rect->y > WINDOW_H-frameHeight)
    {
        rect->x = 0;
        rect->y = WINDOW_H-frameHeight;
    }
    else if (rect->x > WINDOW_W-frameWidth && rect->y > WINDOW_H -frameHeight)
    {
        rect->x = WINDOW_W-frameWidth;
        rect->y = WINDOW_H -frameHeight;
    }
    else if (rect->x < 0)
    {
        rect->x = 0;
    }
    else if (rect->x > WINDOW_W -frameWidth)
    {
        rect->x = WINDOW_W -frameWidth;
    }
    else if (rect->y < 0)
    {
        rect->y = 0;
    }
    else if (rect->y > WINDOW_H-frameHeight)
    {
        rect->y = WINDOW_H-frameHeight;
    }
}

bool DoBoxesIntersect(SDL_Rect* a, SDL_Rect* b) 
{
  return (abs(a->x - b->x) * 2 < (a->w + b->w)) &&
         (abs(a->y - b->y) * 2 < (a->h + b->h));
}