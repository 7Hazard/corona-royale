#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>

#ifdef _WIN32
#include "Windows.h"
#endif

#include "memory.h"
#include "collision.h"
#include "events.h"
#include "player.h"
#include "game.h"
#include "audio.h"
#include "shared/network.h"
#include "shared/data.h"

int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    const int fps = 60;
    const int frameDelay = 1000/fps;
    Uint32 frameStart;
    int frameTime;

    Game* game = GetGame();
    FC_Font* font = FC_CreateFont();  
    FC_LoadFont(font, game->renderer, "res/fonts/ComicSansMS3.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_BOLD|TTF_STYLE_ITALIC);
    
    Network* network = GetNetwork();
    if(!ConnectTCP("localhost"))
    {
        SDL_Log("COULD NOT CONNECT TO GAME SERVER");
    }
    
    // Alloc text
    const char* text = "hello server";
    if(!SendTCPMessage(text, strlen(text)+1))
    {
        SDL_Log("Could not send message to server");
        abort();
    }
    
    {
        uint16_t len = GetTCPMessageLength();
        if(len == 0)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "TCP read failed",
                "Could not get length of player data",
                NULL
            );
            abort();
        }
        
        PlayerData data;
        if(!ReadTCPMessage(&data, len))
        {
            SDL_Log("COULD NOT READ PLAYER DATA");
            abort();
        }

        game->player.position.x = data.x;
        game->player.position.y = data.y;
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

            { // Draw CORONA ROYALE text
                static Uint8 r = 0;
                static float theta = 0.f; theta+=0.03f;
                r = ((sin(theta)+1)/2)*255;
                FC_DrawColor(font, game->renderer, 200, 50, FC_MakeColor(r, 20, 20, 255), "CORONA\n%s", "ROYALE");
            }

            SDL_RenderPresent(game->renderer);
        } /////////// RENDERING PHASE END ///////////
        
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    FC_FreeFont(font);

    IMG_Quit();
    SDL_Quit();
    StopAudio();
    return 0;
}

