#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "game.h"

void HandleBorders(SDL_Rect* rect)
{
    Game* game = GetGame();
    Player* player = &game->player;

    //TOP LEFT BORDER COLLISION
    if (rect->x < 0 && rect->y < 0)
    {
        rect->x = 0;
        rect->y = 0;
    }
    //TOP RIGHT BORDER COLLISION
    else if (rect->x > (WINDOW_W-player->frameWidth) && rect->y < 0)
    {
        rect->x = (WINDOW_W-player->frameWidth);
        rect->y = 0;
    }
    //BOTTOM LEFT BORDER COLLISION
    else if (rect->x < 0 && rect->y > (WINDOW_H-player->frameHeight))
    {
        rect->x = 0;
        rect->y = WINDOW_H-player->frameHeight;
    }
    //BOTTOM RIGHT BORDER COLLISION
    else if (rect->x > WINDOW_W-player->frameWidth && rect->y > WINDOW_H -player->frameHeight)
    {
        rect->x = WINDOW_W-player->frameWidth;
        rect->y = WINDOW_H -player->frameHeight;
    }
    // X-AXIS COLLISION
    else if (rect->x < 0)
    {
        rect->x = 0;
    }
    else if (rect->x > WINDOW_W -player->frameWidth)
    {
        rect->x = WINDOW_W -player->frameWidth;
    }
    //X_AXIS COLLISION

    //Y_AXIS COLLISION
    else if (rect->y < 0)
    {
        rect->y = 0;
    }
    else if (rect->y > WINDOW_H-player->frameHeight)
    {
        rect->y = WINDOW_H-player->frameHeight;
    }
    //Y_AXIS COLLISION
}

bool DoBoxesIntersect(SDL_Rect* a, SDL_Rect* b) 
{
  return (abs(a->x - b->x) * 2 < (a->w + b->w)) &&
         (abs(a->y - b->y) * 2 < (a->h + b->h));
}