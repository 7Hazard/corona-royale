#pragma once
#include <SDL_net.h>

typedef struct Network
{
    TCPsocket tcpsock;
} Network;

Network* GetNetwork();
