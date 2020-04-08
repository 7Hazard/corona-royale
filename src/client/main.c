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

int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    const int fps = 60;
    const int frameDelay = 1000/fps;
    Uint32 frameStart;
    int frameTime;

    Game* game = GetGame();

    //// EXAMPLE
    FC_Font* font = FC_CreateFont();  
    FC_LoadFont(font, game->renderer, "res/fonts/ComicSansMS3.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_BOLD|TTF_STYLE_ITALIC);
    //// EXAMPLE

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

        { /////////// STATE UPDATES PHASE BEGIN ///////////
            HandleEvents();
            OnPlayerUpdate(&game->player);
        } /////////// STATE UPDATES PHASE END /////////////
        
        { /////////// RENDERING PHASE BEGIN /////////
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

            //// EXAMPLE
            {
                static Uint8 r = 0;
                static float theta = 0.f; theta+=0.03f;
                r = ((sin(theta)+1)/2)*255;
                FC_DrawColor(font, game->renderer, 200, 50, FC_MakeColor(r, 20, 20, 255), "CORONA\n%s", "ROYALE");
            }
            //// EXAMPLE

            SDL_RenderPresent(game->renderer);
        } /////////// RENDERING PHASE END ///////////

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    //// EXAMPLE
    FC_FreeFont(font);
    //// EXAMPLE

    // SDL_DestroyTexture(currentImage);
    // SDL_DestroyTexture(background);
    IMG_Quit();
    SDL_Quit();
    StopAudio();
    return 0;
}

