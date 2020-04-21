#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <stdbool.h>

#include "network.h"

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

#ifdef CR_SERVER
        IPaddress ip;
        // create a listening TCP socket on port 6969 (server)

        if(SDLNet_ResolveHost(&ip, NULL, PORT) == -1) {
            printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
            abort();
            exit(2);
        }

        network.tcpsock = SDLNet_TCP_Open(&ip);
#else
        network.tcpsock = NULL;
#endif
    }
    
    return &network;
}

#ifdef CR_CLIENT
bool ConnectTCP(const char* host)
{
    Network* network = GetNetwork();

    IPaddress ip;

    if(SDLNet_ResolveHost(&ip, host, PORT) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    
    network->tcpsock = SDLNet_TCP_Open(&ip);

    if(network->tcpsock == 0) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return false;
    }

    return true;
}
#endif

#ifdef CR_CLIENT
bool SendTCPMessage(uint8_t* content, uint16_t contentLength)
{
    Network* network = GetNetwork();
    TCPsocket* socket = network->tcpsock;
#else
bool SendTCPMessage(TCPsocket* socket, uint8_t* content, uint16_t contentLength)
{
#endif

    // Stack alloc array of bytes
    size_t msglen = contentLength+2; // length of message + 2 bytes for uint16_t
    uint8_t* msg = alloca(msglen);

    // Interpret first 2 bytes as 16-bit unsigned int
    *((uint16_t*)(&msg[0])) = contentLength;
    // Copy contents into message starting from 3rd byte
    memcpy(&msg[2], content, contentLength);

    // Send msg through socket
    size_t sentBytes = SDLNet_TCP_Send(socket,msg,msglen);
    if(sentBytes < msglen)
    {
        // SDL_Log("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        return false;
    }

    return true;
}

#ifdef CR_CLIENT
uint16_t GetTCPMessageLength()
{
    Network* network = GetNetwork();
    TCPsocket* socket = network->tcpsock;
#else
uint16_t GetTCPMessageLength(TCPsocket* socket)
{
#endif

    // Get length of content
    uint16_t contentlen = 0;
    if(SDLNet_TCP_Recv(socket, &contentlen, 2) <= 0) {
        //if zero of less then or equal to zero,then an error occured or the remote host has closed conct.
        // An error may have occured, but sometimes you can just ignore it
        // It may be good to disconnect sock because it is likely invalid now.
        return 0;
    }
    else return contentlen;
}

// Call GetTCPMessageLength before RecieveTCPMessage!!!
#ifdef CR_CLIENT
bool ReadTCPMessage(uint8_t* buffer, uint16_t len)
{
    Network* network = GetNetwork();
    TCPsocket* socket = network->tcpsock;
#else
bool ReadTCPMessage(TCPsocket* socket, uint8_t* buffer,uint16_t len)
{
#endif

    // Read content
    if(SDLNet_TCP_Recv(socket,buffer,len) <= 0)
    {
        return false;
    }

    return true;
}
