#include "player.h"

void CreatePlayer(Player* player)
{
    player->infected = false;
    player->rect.x = 0;
    player->rect.y = 0;
    player->rect.w = 0;
    player->rect.h = 0;
    player->playerPosition.x = 0;
    player->playerPosition.y = 0;
}
