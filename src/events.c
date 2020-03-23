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
    }
}