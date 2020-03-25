#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>

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

int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    if(SDL_Init(0) != 0)
    {
        printf("Could not initialize SDL2");
    }

    window = SDL_CreateWindow("Corona Royale",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,800,0);
    SDL_SetWindowBordered(&window,SDL_TRUE);
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

        HandleEvents();
        HandleBorders(&player.rect);
        HandleBorders(&player2.rect);

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

        if(DoBoxesIntersect(&player.rect, &player2.rect))
        {
            player2.infected = true;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000/600);
    }

    SDL_Quit();
    return 0;
}

