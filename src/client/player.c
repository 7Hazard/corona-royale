#include <SDL_image.h>

#include "player.h"
#include "events.h"
#include "audio.h"
#include "game.h"
#include "collision.h"

void CreatePlayer(Player* player, int xPos, int yPos)
{
    Game *game = GetGame();

    player->image = IMG_LoadTexture(game->renderer, "res/User2.png");
    SDL_QueryTexture(player->image, NULL, NULL, &player->textureWidth, &player->textureHeight);
    player->frameWidth = (player->textureWidth);
    player->frameHeight = (player->textureHeight);
    player->infected = true;
    player->rect.x = 0;
    player->rect.y = 0;
    player->rect.w = player->frameWidth;
    player->rect.h = player->frameHeight;
    player->position.x = xPos;
    player->position.y = yPos;
    player->position.w = player->frameWidth;
    player->position.h = player->frameHeight;
    player->camera.drawingRect.w = player->frameWidth;
    player->camera.drawingRect.h = player->frameHeight;
    player->camera.cameraRect.w = WINDOW_W;
    player->camera.cameraRect.h = WINDOW_H;
    player->mouse.x = 0;
    player->mouse.y = 0;
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
        player->position.y-=7;
    }
    if(player->down == true)
    {
        player->position.y+=7;
    }
    if(player->left == true)
    {
        player->position.x-=7;
    }
    if(player->right == true)
    {
        player->position.x+=7;
    }
   
    if (IsPlayerMoving(player) && !Mix_Playing(1))
    {
        Mix_PlayChannel(1, audio->steps, 0);
    }

    //make player centered on the screen
    player->camera.cameraRect.x = (player->position.x + player->textureWidth/2) - WINDOW_W/2;
    player->camera.cameraRect.y = (player->position.y + player->textureHeight/2) - WINDOW_H/2;


    //background rendering boundries
    if (player->camera.cameraRect.x < 0)
    {
        player->camera.cameraRect.x = 0;
    }
    if (player->camera.cameraRect.y < 0)
    {
        player->camera.cameraRect.y = 0;
    }

    // prevent background from stretching when going beyond boundries
    if (player->camera.cameraRect.x + player->camera.cameraRect.w > game->mapWidth)
    {
        player->camera.cameraRect.x = game->mapWidth - player->camera.cameraRect.w;
    }
    if (player->camera.cameraRect.y + player->camera.cameraRect.h> game->mapHeight)
    {
        player->camera.cameraRect.y = game->mapHeight - player->camera.cameraRect.h;
    }
    
    
    player->camera.drawingRect.x = player->position.x - player->camera.cameraRect.x;
    player->camera.drawingRect.y = player->position.y - player->camera.cameraRect.y;
}

void OnPlayerRender(Player* player)
{
    Game* game = GetGame();
    
    SDL_RenderCopy(game->renderer, player->image, &player->rect, &player->camera.drawingRect);
}

bool IsPlayerMoving(Player* player)
{
    return player->up || player->down || player->left || player->right;
}
