#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
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
#include "timer.h"
#include "shared/network.h"
#include "shared/data.h"
#include "shared/log.h"
#include "shared/netevent.h"

int NetEventThread(void *ptr)
{
    Game* game = GetGame();
    Network* net = GetNetwork();

    while (game->connected)
    {
        NetEvent event = GetNetEvent();
        switch(event)
        {
        case CR_NETEVENT_DISCONNECTED:
        {
            LogInfo("DISCONNECTED");
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_INFORMATION,
                "Connection",
                "Disconnected from server",
                NULL
            );

            return 0;
            break;
        }
        case CR_NETEVENT_PLAYER_CONNECTED:
        {
            PlayerConnectedEvent e;
            ReadPlayerConnectedEvent(net->tcpSocket, &e);
            LogInfo("PLAYER CONNECTED: ID: %d | x: %d | y: %d | angle: %f | inf: %d", e.data.id, e.data.x, e.data.y, e.data.angle, e.data.infected);
            
            break;
        }
        
        default:
            LogInfo("Unhandled event %d", event);
            break;
        }
    }

    return 0;
}

int NetworkThread(void *ptr)
{
    Game* game = GetGame();
    Network* net = GetNetwork();

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
    if(!SendTCPMessage(net->tcpSocket, text, strlen(text)+1))
    {
        SDL_Log("Could not send message to server");
        abort();
    }

    { // Get confirmation
        uint16_t len = GetTCPMessageLength(net->tcpSocket);
        if(len == 0)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "TCP read failed",
                "Could not get confirmation from server",
                NULL
            );
            abort();
        }
        
        char* confirmation = alloca(len);
        if(!ReadTCPMessage(net->tcpSocket, confirmation, len))
        {
            SDL_Log("COULD NOT READ CONFIRMATION");
            abort();
        }

        if(strcmp(confirmation, "OK") != 0)
        {
            SDL_ShowSimpleMessageBox(
                SDL_MESSAGEBOX_ERROR,
                "Connection failed",
                confirmation,
                NULL
            );
            LogInfo("CONNECTION FAILED: %s", confirmation);

            return 1;
        }
        else
        {
            LogInfo("CONNECTED TO SERVER: %s", confirmation);
        }
    }

    { // Get other players
        uint16_t datasize = GetTCPMessageLength(net->tcpSocket);
        uint16_t count = GetTCPMessageLength(net->tcpSocket);

        PlayerData* data = alloca(datasize*count);
        if(!ReadTCPMessageArray(net->tcpSocket, data, datasize, count))
        {
            abort();
        }

        for (size_t i = 0; i < count; i++)
        {
            LogInfo("ID: %d | x: %d | y: %d | angle: %f | inf: %d", data[i].id, data[i].x, data[i].y, data[i].angle, data[i].infected);
        }
    }
    
    { // Get player data
        uint16_t len = GetTCPMessageLength(net->tcpSocket);
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
        if(!ReadTCPMessage(net->tcpSocket, &data, len))
        {
            SDL_Log("COULD NOT READ PLAYER DATA");
            abort();
        }

        ApplyPlayerData(&game->player, &data);
    }

    game->connected = true;

    // Start event thread
    SDL_Thread* eventThread = SDL_CreateThread(NetEventThread, "NetEventThread", (void *)NULL);

    // Network loop
    while (game->connected)
    {
        time_t start = clock();
        ///////// START OF NET TICK

        // Send 
        PlayerPositionData data;
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
    SDL_Log("Corona Royale\n");

    const int fps = 60;
    const int frameDelay = 1000/fps;
    Uint32 frameStart;
    int frameTime;

    Game* game = GetGame();
    // FC_Font* font = FC_CreateFont();
    // FC_LoadFont(font, game->renderer, "res/fonts/ComicSansMS3.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_BOLD|TTF_STYLE_ITALIC);
    
    // Start network thread
    SDL_Thread* networkThread = SDL_CreateThread(NetworkThread, "NetworkThread", (void *)NULL);

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

