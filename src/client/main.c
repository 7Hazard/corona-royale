#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_mixer.h>
#include "collison.h"
#include "events.h"
#include "player.h"
#include "audio.h"

int running = 1;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int main(int argc, const char *argv[])
{
    SDL_Log("Corona Royale\n");
    const int fps = 60;
    const int frameDelay = 1000/fps;

    Uint32 frameStart;
    int frameTime;

    if(SDL_Init(SDL_INIT_AUDIO)!=0) 
    {
        SDL_Log("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    if(Mix_Init(0)!=0) 
    {
        SDL_Log("Mix_Init: %s\n", SDL_GetError());
        exit(1);
    }
    InitAudio();

    window = SDL_CreateWindow("Corona Royale",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,800,0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    Player player;
    CreatePlayer(&player);
    player.rect.x = 200;
    player.rect.y = 200;
    player.rect.w = 80;
    player.rect.h = 80;
    player.infected = true;

    Player player2;
    CreatePlayer(&player2);
    player2.rect.x = 300;
    player2.rect.y = 300;
    player2.rect.w = 80;
    player2.rect.h = 80;

    while (running)
    {
        frameStart = SDL_GetTicks();

        HandleEvents();
        HandleBorders(&player.rect);
        HandleBorders(&player2.rect);
        OnPlayerUpdate(&player);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        if(DoBoxesIntersect(&player.rect, &player2.rect) && player2.infected != true)
        {
            Mix_PlayChannel(-1, cough, 0);
            player2.infected = true;
        }

        OnPlayerRender(&player);
        OnPlayerRender(&player2);
        
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay >frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    
    StopAudio();
    SDL_Quit();
    return 0;
}

