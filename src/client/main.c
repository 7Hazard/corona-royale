#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "collision.h"
#include "events.h"
#include "player.h"
#include "game.h"
#include "audio.h"
#include "timer.h"


int main(int argc, const char *argv[])
{
    SDL_Log("Corona Royale\n");

    const int fps = 60;
    const int frameDelay = 1000/fps;
    Uint32 frameStart;
    int frameTime;

    Game* game = GetGame();

    if (TTF_Init() < 0) { // ej probelm
        printf("Error\n");
        abort();
    }

    while (game->running)
    {

        frameStart = SDL_GetTicks();

        { /////////// STATE UPDATES PHASE BEGIN ///////////
            HandleEvents();
            OnPlayerUpdate(&game->player);
        } /////////// STATE UPDATES PHASE END /////////////
        
        { /////////// RENDERING PHASE BEGIN /////////
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderClear(game->renderer);
            
            // Draw background
            SDL_RenderCopy(game->renderer, game->background, &game->player.camera.cameraRect, NULL);

            OnPlayerRender(&game->player);
            
            RendererTimer(&game->timer);

            //lägg in en till sdl renderer

            SDL_RenderPresent(game->renderer);
        } /////////// RENDERING PHASE END ///////////
        
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }


    IMG_Quit();
    SDL_Quit();
    StopAudio();
    return 0;
}

