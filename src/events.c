#include <stdio.h>
#include <SDL.h>
#include "main.h"

void HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) 
        {
            running = 0;
            SDL_Log("Program quit after %i ticks", event.quit.timestamp);
            break;
        }
        if(event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
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
        if(event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
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
}