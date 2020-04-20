#include <SDL_image.h>

#include "player.h"
#include "events.h"
#include "audio.h"
#include "game.h"
#include "timer.h"
#include "collision.h"

void CreatePlayer(Player* player, int xPos, int yPos)
{
    Game *game = GetGame();

    player->image = IMG_LoadTexture(game->renderer, "res/User.png");
    SDL_QueryTexture(player->image, NULL, NULL, &player->textureWidth, &player->textureHeight);
    player->frameWidth = (player->textureWidth)/4;
    player->frameHeight = (player->textureHeight)/4;
    player->infected = true;
    player->rect.x = 0;
    player->rect.y = 0;
    player->rect.w = player->frameWidth;
    player->rect.h = player->frameHeight;
    player->position.x = xPos;
    player->position.y = yPos;
    player->position.w = player->frameWidth;
    player->position.h = player->frameHeight;
    player->cameraRect.x = 0;
    player->cameraRect.y = 0;
    player->cameraRect.w = WINDOW_W;
    player->cameraRect.h = WINDOW_H;
}

void HandlePlayerEvents(SDL_Event *event)
{
    Game* game = GetGame();
    Player* player = &game->player;

    if(event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_a: player->left = true;
            break;
            case SDLK_d: player->right = true;
            break;
            case SDLK_w: player->up = true;
            break;
            case SDLK_s: player->down = true;
            break;
            case SDLK_m: game->timer.mStarted = true;
            break;
        }
    }
    if(event->type == SDL_KEYUP)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_a: player->left = false;
            break;
            case SDLK_d: player->right = false;
            break;
            case SDLK_w: player->up = false;
            break;
            case SDLK_s: player->down = false;
            break;
        }
    }
}

void OnPlayerUpdate(Player* player)
{
    Audio* audio = GetAudio();
    Game *game = GetGame();
    HandleBorders(player);

    if(player->up == true)
    {
        player->position.y-=1;
        player->rect.y = player->frameHeight;
    }
    if(player->down == true)
    {
        player->position.y+=1;
        player->rect.y = 0;
    }
    if(player->left == true)
    {
        player->position.x-=1;
        player->rect.y = player->frameHeight*2;
    }
    if(player->right == true)
    {
        player->position.x+=1;
        player->rect.y = player->frameHeight*3;
    }
    if (!(player->up && player->down && player->left && player->right))
    {
        if (player->rect.y == player->frameHeight*3)
        {
            player->rect.x = player->frameWidth*3;
        }
        else
        {
            player->rect.x = 0;
        }
    }
    
    if (IsPlayerMoving(player) && !Mix_Playing(1))
    {
        Mix_PlayChannel(1, audio->steps, 0);
    }
    
    if (player->position.x >= WINDOW_W/2)
    {
        player->cameraRect.x = player->position.x - WINDOW_W/2;
    }
    if (player->cameraRect.x + player->cameraRect.w >= game->mapWidth)
    {
        player->cameraRect.x = game->mapWidth-WINDOW_W;
    }
    if (player->position.y >= WINDOW_H/2)
    {
        player->cameraRect.y = player->position.y - WINDOW_H/2;
    }
    if (player->cameraRect.y + player->cameraRect.h >= game->mapHeight)
    {
        player->cameraRect.y = game->mapHeight-WINDOW_H;
    }
}

void OnPlayerRender(Player* player)
{
    Game* game = GetGame();
    
    
    SDL_RenderCopy(game->renderer, player->image, &player->rect, &player->position);
}

bool IsPlayerMoving(Player* player)
{
    return player->up || player->down || player->left || player->right;
}
