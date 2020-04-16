#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "game.h"

void HandleBorders(Player *player)
{
    Game* game = GetGame();
    
    //TOP LEFT BORDER COLLISION
    if (player->position.x < 0 && player->position.y < 0)
    {
        player->position.x = 0;
        player->position.y = 0;
    }
    //TOP RIGHT BORDER COLLISION
    else if (player->position.x > (game->mapWidth-player->frameWidth) && player->position.y < 0)
    {
        player->position.x = (game->mapWidth-player->frameWidth);
        player->position.y = 0;
    }
    //BOTTOM LEFT BORDER COLLISION
    else if (player->position.x < 0 && player->position.y > (game->mapHeight-player->frameHeight))
    {
        player->position.x = 0;
        player->position.y = game->mapHeight-player->frameHeight;
    }
    //BOTTOM RIGHT BORDER COLLISION
    else if (player->position.x > game->mapWidth-player->frameWidth && player->position.y > game->mapHeight -player->frameHeight)
    {
        player->position.x = game->mapWidth-player->frameWidth;
        player->position.y = game->mapHeight -player->frameHeight;
    }
    // X-AXIS COLLISION
    else if (player->position.x < 0)
    {
        player->position.x = 0;
    }
    else if (player->position.x > game->mapWidth -player->frameWidth)
    {
        player->position.x = game->mapWidth -player->frameWidth;
    }
    //X_AXIS COLLISION

    //Y_AXIS COLLISION
    else if (player->position.y < 0)
    {
        player->position.y = 0;
    }
    else if (player->position.y > game->mapHeight-player->frameHeight)
    {
        player->position.y = game->mapHeight-player->frameHeight;
    }
    //Y_AXIS COLLISION
}

bool DoBoxesIntersect(SDL_Rect* a, SDL_Rect* b) 
{
  return (abs(a->x - b->x) * 2 < (a->w + b->w)) &&
         (abs(a->y - b->y) * 2 < (a->h + b->h));
}
