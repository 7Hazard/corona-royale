#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>
#include <time.h> 

#include "memory.h"
#include "collision.h"
#include "events.h"
#include "player.h"
#include "game.h"
#include "audio.h"

void GetGameState(enum States *currentState);
#include "timer.h"
#include "shared/network.h"
#include "shared/data.h"

int NetworkThread(void *ptr)
{
    Game* game = GetGame();
    Network* network = GetNetwork();

    if(!Connect("localhost"))
    {
        SDL_Log("COULD NOT CONNECT TO GAME SERVER");
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Connection error",
            "Could not connect to server",
            NULL
        );
        abort();
    }
    
    // Alloc text
    const char* text = "hello server";
    if(!SendTCPMessage(text, strlen(text)+1))
    {
        SDL_Log("Could not send message to server");
        abort();
    }
    
    { // Get player data
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

        ApplyPlayerData(&game->player, &data);
    }

    // Network loop
    while (1)
    {
        time_t start = clock();
        ///////// START OF NET TICK

        // Send 
        PositionData data;
        GetPlayerPositionData(&game->player, &data);
        SendPositionData_UDP(&data);
        
        ///////// END OF NET TICK
        time_t end = clock();
        int result = end-start;
        if (result < CR_NET_TICK_TIME)
        {
            SDL_Delay (CR_NET_TICK_TIME-result);
        }   
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    enum States currentState; // lagt till
    currentState = CR_STATE_PRELOAD;

    const int fps = 60;
    const int frameDelay = 1000 / fps;
    Uint32 frameStart;
    int frameTime;

    Game* game = GetGame();
    // FC_Font* font = FC_CreateFont();
    // FC_LoadFont(font, game->renderer, "res/fonts/ComicSansMS3.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_BOLD|TTF_STYLE_ITALIC);
    
    // Start network thread
    SDL_Thread* networkThread = SDL_CreateThread(NetworkThread, "NetworkThread", (void *)NULL);

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