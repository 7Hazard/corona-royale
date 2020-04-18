#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <stdbool.h>

#include "network.h"

#define MAX_CONNECTIONS 2
#define PORT 6969

Network* GetNetwork()
{
    static bool inited = false;
    static Network network;

    if(!inited)
    {
        inited = true;

        if (SDLNet_Init() < 0) 
        {
            printf("Couldn't initialize net: %s\n", SDLNet_GetError());
            abort();
            exit(1);
        }

        IPaddress ip;

#ifdef CR_SERVER
        // create a listening TCP socket on port 6969 (server)

        if(SDLNet_ResolveHost(&ip, NULL, PORT) == -1) {
            printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
            abort();
            exit(2);
        }

        network.tcpsock = SDLNet_TCP_Open(&ip);
#else
        if(SDLNet_ResolveHost(&ip, "localhost", PORT) == -1) {
            printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
            abort();
            exit(2);
        }

        network.tcpsock = SDLNet_TCP_Open(&ip);
#endif

        if(!network.tcpsock) {
            printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
            abort();
            exit(3);
        }
        // accept a connection coming in on server_tcpsock
    }
    
    return &network;
}
