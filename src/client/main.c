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
<<<<<<< Updated upstream
=======
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

            { // Draw CORONA ROYALE text
                static Uint8 r = 0;
                static float theta = 0.f; theta+=0.03f;
                r = ((sin(theta)+1)/2)*255;
                FC_DrawColor(font, game->renderer, 200, 50, FC_MakeColor(r, 20, 20, 255), "CORONA\n%s", "ROYALE");
            }

            SDL_RenderPresent(game->renderer);
        } /////////// RENDERING PHASE END ///////////
>>>>>>> Stashed changes
        
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

