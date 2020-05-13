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
#include "textures.h"


#include "shared/data.h"
#include "shared/math.h"
#include "shared/log.h"

void CreatePlayer(Player* player, float xPos, float yPos)
{
    Game *game = GetGame();
    Textures* tex = GetTextures();

    player->texture = tex->healthyPlayer;
    player->rect.w = tex->playerWidth;
    player->rect.h = tex->playerHeight;

    player->infectedMutex = SDL_CreateMutex();
    player->infected = true;

    player->rect.x = 0;
    player->rect.y = 0;

    player->movementMutex = SDL_CreateMutex();
    player->radius = 25;
    player->x = xPos;
    player->y = yPos;
    player->camera.cameraRect.w = CR_WINDOW_W;
    player->camera.cameraRect.h = CR_WINDOW_H;

    player->mouseClick = false;
    player->moving = false;
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
        player->moving = false;
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
    player->angle = GetAngle(player->x + player->rect.w/2, player->y + player->rect.h/2, mousex, mousey)-90;
}

void MoveTowardsMouse(Player *player)
{
    int mouseX, mouseY;
    float distance;
    float originX, originY;
    float directionX, directionY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int d = 15;

    originX  = player->x + player->rect.w/2;
    originY = player->y + player->rect.h/2;
    distance = sqrt(pow((mouseX + player->camera.cameraRect.x)-originX,2)+pow((mouseY + player->camera.cameraRect.y)-originY,2));
    directionX = (mouseX+ player->camera.cameraRect.x-originX)/distance;
    directionY = (mouseY + player->camera.cameraRect.y-originY)/distance;
    
    if ((mouseX +player->camera.cameraRect.x)>= (player->x - d) && ((mouseX +player->camera.cameraRect.x) < (player->x + player->rect.w + d)))
    {
        if (mouseY + player->camera.cameraRect.y>= (player->y - d) && (mouseY + player->camera.cameraRect.y< (player->y + player->rect.h + d)))
        {
            player->moving = false;
            return;
        }
    }

    if (player->moving == true)
    {
        player->x +=directionX*7;
        player->y += directionY*7;
        if (sqrt(pow(player->x-originX,2)+pow(player->y-originY,2)) >= distance)
        {

            player->x = (float)(mouseX + player->camera.cameraRect.x);
            player->y = (float)(mouseY + player->camera.cameraRect.y);
            return;
        }
        
    }
}

void OnPlayerUpdate(Player* player)
{
    Audio* audio = GetAudio();
    Game *game = GetGame();
    
    HandleBorders(player);
    RotatePlayer(player);

    int posx = player->x;
    int posy = player->y;
   
    if (player->mouseClick == true)
    {
        player->moving = true;
        MoveTowardsMouse(player);
    }
    
    if (player->moving == true && !Mix_Playing(1))
    {
        Mix_VolumeChunk(audio->steps,20);
        Mix_PlayChannel(1, audio->steps, 0);
    }

    //make player centered on the screen
    //make the camera scroll depending on the player position
    player->camera.cameraRect.x = (posx + player->rect.w/2) - CR_WINDOW_W/2;
    player->camera.cameraRect.y = (posy + player->rect.h/2) - CR_WINDOW_H/2;

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
void DrawCircle(Player* player)
{
    Game* game = GetGame();
    const int32_t diameter = (player->radius * 2);

    int32_t x = (player->radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);
    int centreX = ((int)player->x + player->rect.w/2) - player->camera.cameraRect.x;
    int centreY = ((int)player->y + player->rect.h/2) - player->camera.cameraRect.y;
    SDL_SetRenderDrawColor(game->renderer, 255,0,0,255);

    while (x >= y) 
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(game->renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(game->renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(game->renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(game->renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(game->renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(game->renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(game->renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(game->renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void OnPlayerRender(Player* player)
{
    Game* game = GetGame();
    static int frameTime = 0;
    
    SDL_RenderCopyEx(game->renderer, player->texture, NULL, &player->rect, player->angle, NULL, SDL_FLIP_NONE);
    if (player->infected)
    {
        if (player->radius <= 50)
        {
            DrawCircle(player);
            if (frameTime == 30)
            {
                player->radius = player->radius + 5;
                frameTime = 0;
            }
            
            if (player->radius == 40)
            {
                player->radius = 25;
            }
            frameTime++;   
        }
    }
}

bool IsPlayerMoving(Player* player)
{
    assert(false, "UNIMPLEMENTED");
    return false;
}

void SetPlayerPosition(Player* player, int x, int y)
{
    SDL_LockMutex(player->movementMutex);
    player->x = x;
    player->y = y;
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
    Textures* textures = GetTextures();

    SDL_LockMutex(player->infectedMutex);
    player->infected = infected;
    if(infected) player->texture = textures->infectedPlayer;
    else player->texture = textures->healthyPlayer;
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
    data->x = player->x;
    data->y = player->y;
}
