#include <SDL_image.h>
#include <SDL.h>
#include <stdio.h>
#include <assert.h>

#include "player.h"
#include "events.h"
#include "audio.h"
#include "game.h"
#include "timer.h"
#include "textures.h"
#include "collision.h"
#include "mouse.h"
#include "textures.h"

#include "shared/data.h"

void CreatePlayer(Player* player, int xPos, int yPos)
{
    Game *game = GetGame();
    Textures* tex = GetTextures();

    player->texture = tex->healthyPlayer;
    player->frameWidth = tex->playerWidth;
    player->frameHeight = tex->playerHeight;

    player->infectedMutex = SDL_CreateMutex();
    player->infected = true;

    player->rect.x = 0;
    player->rect.y = 0;
    player->rect.w = player->frameWidth;
    player->rect.h = player->frameHeight;

    player->movementMutex = SDL_CreateMutex();
    player->position.x = xPos;
    player->position.y = yPos;
    player->position.w = player->frameWidth;
    player->position.h = player->frameHeight;

    player->camera.cameraRect.w = WINDOW_W;
    player->camera.cameraRect.h = WINDOW_H;

    player->mouseClick = false;
}

void HandlePlayerEvents(SDL_Event *event)
{
    Game* game = GetGame();
    Player* player = &game->player;

    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        player->mouseClick = true;
    }
    if (event->type == SDL_MOUSEBUTTONUP)
    {
        player->mouseClick = false;
    }
}

void RotatePlayer(Player *player)
{
    Game* game = GetGame();
    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);
    //Get "world" coordinates instead of windowpos
    mousex += player->camera.cameraRect.x;
    mousey += player->camera.cameraRect.y;
    player->angle = GetAngle(player->position.x + player->rect.w/2, mousex, player->position.y + player->rect.h/2, mousey);
}

void MoveTowardsMouse(Player *player)
{
    int mousex, mousey;
    int newPosX, newPosY;
    SDL_GetMouseState(&mousex, &mousey);
    
    newPosX = (mousex + player->camera.cameraRect.x)- player->position.x + player->rect.w/2;
    newPosY = (mousey + player->camera.cameraRect.y) - player->position.y + player->rect.h/2;
    player->position.x += newPosX/20;
    player->position.y +=newPosY/20;
}

void OnPlayerUpdate(Player* player)
{
    Audio* audio = GetAudio();
    Game *game = GetGame();
    
    HandleBorders(player);
    RotatePlayer(player);

    int posx = player->position.x;
    int posy = player->position.y;
   
    if (player->mouseClick == true)
    {
        MoveTowardsMouse(player);
    }
    
    if (player->mouseClick == true && !Mix_Playing(1))
    {
        Mix_PlayChannel(1, audio->steps, 0);
    }

    //make player centered on the screen
    //make the camera scroll depending on the player position
    player->camera.cameraRect.x = (posx + player->textureWidth/2) - WINDOW_W/2;
    player->camera.cameraRect.y = (posy + player->textureHeight/2) - WINDOW_H/2;

    //background rendering boundries
    if (player->camera.cameraRect.x < 0)
    {
        player->camera.cameraRect.x = 0;
    }
    if (player->camera.cameraRect.y < 0)
    {
        player->camera.cameraRect.y = 0;
    }

    // prevent background image from stretching when going beyond boundries
    if (player->camera.cameraRect.x + player->camera.cameraRect.w > game->mapWidth)
    {
        player->camera.cameraRect.x = game->mapWidth - player->camera.cameraRect.w;
    }
    if (player->camera.cameraRect.y + player->camera.cameraRect.h> game->mapHeight)
    {
        player->camera.cameraRect.y = game->mapHeight - player->camera.cameraRect.h;
    }
    
    // render the player centered in camera
    player->rect.x = posx - player->camera.cameraRect.x;
    player->rect.y = posy - player->camera.cameraRect.y;
}

void OnPlayerRender(Player* player)
{
    Game* game = GetGame();
    SDL_RenderCopyEx(game->renderer, player->texture, NULL, &player->rect, player->angle, NULL, SDL_FLIP_NONE);
}

bool IsPlayerMoving(Player* player)
{
    assert(false, "UNIMPLEMENTED");
    return false;
}

void SetPlayerPosition(Player* player, int x, int y)
{
    SDL_LockMutex(player->movementMutex);
    player->position.x = x;
    player->position.y = y;
    SDL_UnlockMutex(player->movementMutex);
}

void SetPlayerAngle(Player* player, float angle)
{
    SDL_LockMutex(player->movementMutex);
    player->angle = angle;
    SDL_UnlockMutex(player->movementMutex);
}

void SetPlayerInfected(Player* player, bool infected)
{
    SDL_LockMutex(player->infectedMutex);
    player->infected = infected;
    SDL_UnlockMutex(player->infectedMutex);
}

void ApplyPlayerData(Player* player, PlayerData* data)
{
    player->id = data->id;
    SetPlayerPosition(player, data->x, data->y);
    SetPlayerAngle(player, data->angle);
    SetPlayerInfected(player, data->infected);
}

void GetPlayerMovementData(Player* player, PlayerMovementData* data)
{
    data->id = player->id;
    data->angle = player->angle;
    data->x = player->position.x;
    data->y = player->position.y;
}
