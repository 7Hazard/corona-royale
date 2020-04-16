#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "collision.h"
#include "events.h"
#include "player.h"
#include "game.h"
#include "audio.h"

int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    const int fps = 60;
    const int frameDelay = 1000/fps;
    Uint32 frameStart;
    int frameTime;

    Game* game = GetGame();

    while (game->running)
    {
        
        frameStart = SDL_GetTicks();

        HandleEvents();
        OnPlayerUpdate(&game->player);
        
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        
        // Draw background
        OnPlayerRender(&game->player);
        SDL_RenderCopy(game->renderer, game->background, &game->player.camera.cameraRect, NULL);
        

        OnPlayerRender(&game->player);
        
        SDL_RenderPresent(game->renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // SDL_DestroyTexture(currentImage);
    // SDL_DestroyTexture(background);
    IMG_Quit();
    SDL_Quit();
    StopAudio();
    return 0;
}

