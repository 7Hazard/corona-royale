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
        // if (IsPlayerMoving(player) == false)
        // {
        //     HandleEvents(&player);

        //     //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //     if (frameTime == 60)
        //     {
        //         frameTime = 0;
        //         player.rect.x += frameWidth;
        //         if (player.rect.x >= textureWidth)
        //         {
        //             player.rect.x = 0;
        //         }
                
        //     }

        //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////

        frameStart = SDL_GetTicks();

        HandleEvents();
        OnPlayerUpdate(&game->player);
        
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        
        // Draw background
        SDL_RenderCopy(game->renderer, game->background, &game->player.cameraRect, NULL);
        
        // if(DoBoxesIntersect(&player.rect, &player2.rect) && player2.infected != true)
        // {
        //     Mix_PlayChannel(-1, audio->cough, 0);
        //     player2.infected = true;
        // }

        OnPlayerRender(&game->player);
        //OnPlayerRender(&player2);
        
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

