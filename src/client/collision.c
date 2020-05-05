#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "game.h"

void HandleBorders(Player *player)
{
    Game* game = GetGame();
    
    //TOP LEFT BORDER COLLISION
    if (player->x < 0 && player->y < 0)
    {
        player->x = 0;
        player->y = 0;
    }
    //TOP RIGHT BORDER COLLISION
    else if (player->x > (game->mapWidth-player->rect.w) && player->y < 0)
    {
        player->x = (game->mapWidth-player->rect.w);
        player->y = 0;
    }
    //BOTTOM LEFT BORDER COLLISION
    else if (player->x < 0 && player->y > (game->mapHeight-player->rect.h))
    {
        player->x = 0;
        player->y = game->mapHeight-player->rect.h;
    }
    //BOTTOM RIGHT BORDER COLLISION
    else if (player->x > game->mapWidth-player->rect.w && player->y > game->mapHeight -player->rect.h)
    {
        player->x = game->mapWidth-player->rect.w;
        player->y = game->mapHeight -player->rect.h;
    }
    // X-AXIS COLLISION
    else if (player->x < 0)
    {
        player->x = 0;
    }
    else if (player->x > game->mapWidth -player->rect.w)
    {
        player->x = game->mapWidth -player->rect.w;
    }
    //X_AXIS COLLISION

    //Y_AXIS COLLISION
    else if (player->y < 0)
    {
        player->y = 0;
    }
    else if (player->y > game->mapHeight-player->rect.h)
    {
        player->y = game->mapHeight-player->rect.h;
    }
    //Y_AXIS COLLISION
}

bool DoBoxesIntersect(SDL_Rect* a, SDL_Rect* b) 
{
  return (abs(a->x - b->x) * 2 < (a->w + b->w)) &&
         (abs(a->y - b->y) * 2 < (a->h + b->h));
}
