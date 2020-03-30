#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_mixer.h>
#include "collison.h"
#include "events.h"
#include "player.h"

int running = 1;
bool pressed_w = false;
bool pressed_s = false;
bool pressed_a = false;
bool pressed_d = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Mix_Chunk *walking = NULL;
Mix_Chunk *cough = NULL;
Mix_Chunk *steps = NULL;

int main(int argc, const char *argv[])
{
    SDL_Log("Corona Royale\n");
    const int fps = 144;
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
    
    window = SDL_CreateWindow("Corona Royale",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,800,0);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); 	
    // Mix_Music *backgroundSound = Mix_LoadMUS("res/Heroic Demise (New).mp3");
    cough = Mix_LoadWAV("res/cough.wav");
    walking = Mix_LoadWAV("res/Ejimas1.ogg");
    steps =  Mix_LoadWAV("res/footstep.wav");
    // Mix_PlayMusic(backgroundSound, -1);

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
        if(pressed_w == true)
        {
            player.rect.y-=1;
        }
        if(pressed_s == true)
        {
            player.rect.y+=1;
        }
        if(pressed_a == true)
        {
            player.rect.x-=1;
        }
        if(pressed_d == true)
        {
            player.rect.x+=1;
        }
    
        if ((pressed_w || pressed_s||pressed_a||pressed_d)&& !Mix_Playing(1))
        {
              Mix_PlayChannel(1,steps, 1);
        }
                // Mix_PlayChannel(-1, player.walking, 0);

        HandleEvents();
        HandleBorders(&player.rect);
        HandleBorders(&player2.rect);
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay >frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if(player.infected)
        {
            SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        SDL_RenderFillRect(renderer,&player.rect);

        if(player2.infected)
        {
            SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        SDL_RenderFillRect(renderer,&player2.rect);
        
        if(DoBoxesIntersect(&player.rect, &player2.rect) && player2.infected != true)
        {
            Mix_PlayChannel(-1, cough, 0);
            player2.infected = true;
        }
        
        
        SDL_RenderPresent(renderer);
    }
    
    // Mix_FreeChunk(walking);
    // Mix_FreeChunk(cough);
    // Mix_FreeMusic(backgroundSound);
    Mix_FreeChunk(steps);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    return 0;
}

