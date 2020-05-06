#include <stdio.h>
#include <SDL.h>
#include "game.h"
#include "player.h"


void HandleEvents()
{
    Game* game = GetGame();


    while (SDL_PollEvent(&game->event))
    {
        if (game->event.type == SDL_QUIT) 
        {
            game->running = 0;
            SDL_Log("Program quit after %i ticks", game->event.quit.timestamp);
            break;
        }
        HandlePlayerEvents(&game->event); 

        if(game->currentState == CR_STATE_MENU){
            HandleMenuEvents(&game->event);
        }

    }
    
    
}

