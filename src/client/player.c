#include "player.h"

void CreatePlayer(Player* player, int xPos, int yPos,int frameWidth, int frameHeight)
{
    player->infected = true;
    player->noMovement = false;
    player->rect.x = 0;
    player->rect.y = 0;
    player->rect.w = frameWidth;
    player->rect.h = frameHeight;
    player->playerPosition.x = xPos;
    player->playerPosition.y = yPos;
    player->playerPosition.w = frameWidth;
    player->playerPosition.h = frameHeight;
}
