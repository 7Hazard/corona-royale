#include <stdio.h>
#include <SDL.h>
#include "main.h"
#include "player.h"

void HandleEvents(Player *player)
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
        if(event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
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
}

