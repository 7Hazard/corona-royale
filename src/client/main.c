#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>

#include "collision.h"
#include "events.h"
#include "player.h"
#include "game.h"
#include "audio.h"

void GetGameState(enum States *currentState);

int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    enum States currentState; // lagt till
    currentState = CR_STATE_PRELOAD;

    const int fps = 60;
    const int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;

    Game *game = GetGame();

    currentState = CR_STATE_MENU;
    while (game->running)
    {

        frameStart = SDL_GetTicks();
        //Get gamestate

        { /////////// STATE UPDATES PHASE BEGIN ///////////
            HandleEvents();
            OnPlayerUpdate(&game->player);
        } /////////// STATE UPDATES PHASE END /////////////

        { /////////// RENDERING PHASE BEGIN /////////
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderClear(game->renderer);

            if(currentState == CR_STATE_RUNNING)
            {
                // Draw background
                SDL_RenderCopy(game->renderer, game->background, &game->player.camera.cameraRect, NULL);

                OnPlayerRender(&game->player);
                RendererTimer(&game->timer);

                SDL_RenderCopy(game->renderer, game->menu.textureMenu, NULL, NULL);
            } else if(currentState == CR_STATE_MENU)
            {
                RenderMenu();
            }

            SDL_RenderPresent(game->renderer);
            
        } /////////// RENDERING PHASE END ///////////

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    SDL_DestroyTexture(game->background);
    IMG_Quit();
    SDL_Quit();
    StopAudio();
    return 0;
}

void GetGameState(enum States *currentState)
{
    switch (*currentState)
    {
    case CR_STATE_MENU:

        break;

    default:
        break;
    }
}