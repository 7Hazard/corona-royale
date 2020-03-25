#include <stdio.h>
#include <stdbool.h>
#include "collisonborder.h"
#include <SDL.h>
#include "events.h"

int running = 1;
bool pressed_w = false;
bool pressed_s = false;
bool pressed_a = false;
bool pressed_d = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect rect;
int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    if(!SDL_Init(0))
    {
        printf("Could not initialize SDL2");
    }
    window = SDL_CreateWindow("Corona Royale",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,800,0);
    SDL_SetWindowBordered(&window,SDL_TRUE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    rect.x = 200;
    rect.y = 200;
    rect.w = 80;
    rect.h = 80;
    while (running)
    {
        if(pressed_w == true)
        {
            rect.y-=1;
        }
        if(pressed_s == true)
        {
            rect.y+=1;
        }
        if(pressed_a == true)
        {
            rect.x-=1;
        }
        if(pressed_d == true)
        {
            rect.x+=1;
        }

        HandleEvents();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        HandleBorder();

        SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
        SDL_RenderFillRect(renderer,&rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/600);
    }

    SDL_Quit();
    return 0;
}

