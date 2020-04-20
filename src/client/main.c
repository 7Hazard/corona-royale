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
        printf("Kuksugare");
    }

    while (game->running)
    {
        frameStart = SDL_GetTicks();
        UpdateTimer(&game->timer);
        CheckTime(&game->timer);

        { /////////// STATE UPDATES PHASE BEGIN ///////////
            HandleEvents();
            OnPlayerUpdate(&game->player);
        } /////////// STATE UPDATES PHASE END /////////////
        
        { /////////// RENDERING PHASE BEGIN /////////
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderClear(game->renderer);
            
            // Draw background
            SDL_RenderCopy(game->renderer, game->background, &game->player.cameraRect, NULL);

            OnPlayerRender(&game->player);
            
            UpdateTimer(&game->timer);
            sprintf(game->timer.timerBuffer,"%d:%d", game->timer.mMinuteTime,game->timer.mDeltaTime); // copy string (timer.mStartTicks)to buffer

            if(!game->timer.mLast10Sek){
                game->timer.surfaceTime = TTF_RenderText_Solid(game->timer.Sans,game->timer.timerBuffer ,game->timer.White);
            }else if(game->timer.mLast10Sek){
                game->timer.surfaceTime = TTF_RenderText_Solid(game->timer.Sans,game->timer.timerBuffer ,game->timer.Red);
            }

            game->timer.textureTime = SDL_CreateTextureFromSurface(game->renderer, game->timer.surfaceTime); //now you can convert it into a texture
            SDL_RenderCopy(game->renderer, game->timer.textureTime, NULL, &game->timer.rect);   
    

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

