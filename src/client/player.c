#include "player.h"
#include "events.h"
#include "main.h"
#include "audio.h"

bool pressed_w = false;
bool pressed_s = false;
bool pressed_a = false;
bool pressed_d = false;

void CreatePlayer(Player* player)
{
    player->infected = false;
    player->rect.x = 0;
    player->rect.y = 0;
    player->rect.w = 0;
    player->rect.h = 0;
}

void HandlePlayerEvents(SDL_Event *event)
{
    if(event->type == SDL_KEYDOWN)
        {
            // Mix_PlayChannel(-1, walking, 0);
            switch (event->key.keysym.sym)
            {
                case SDLK_a: pressed_a = true;
                break;
                case SDLK_d: pressed_d = true;
                break;
                case SDLK_w: pressed_w = true;
                break;
                case SDLK_s: pressed_s = true;
                break;
            }
        }
    if(event->type == SDL_KEYUP)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_a: pressed_a = false;
            break;
            case SDLK_d: pressed_d = false;
            break;
            case SDLK_w: pressed_w = false;
            break;
            case SDLK_s: pressed_s = false;
            break;
        }
    }
}

void OnPlayerUpdate(Player* player)
{
    if(pressed_w == true)
    {
        player->rect.y-=1;
    }
    if(pressed_s == true)
    {
        player->rect.y+=1;
    }
    if(pressed_a == true)
    {
        player->rect.x-=1;
    }
    if(pressed_d == true)
    {
        player->rect.x+=1;
    }
    
    if ((pressed_w || pressed_s||pressed_a||pressed_d) && !Mix_Playing(1))
    {
        Mix_PlayChannel(1, steps, 1);
    }
}

void OnPlayerRender(Player* player)
{
    if(player->infected)
    {
        SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    }
    SDL_RenderFillRect(renderer, &player->rect);
    
}