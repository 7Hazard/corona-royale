#include <stdio.h>
#include <SDL.h>
#include <SDL_net.h>
#include <stdbool.h>

#include "log.h"
#include "network.h"
#include "server/netplayer.h"

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
        // create a listening TCP socket on port 6969 (server)

        if(SDLNet_ResolveHost(&network.ip, NULL, CR_NET_PORT) == -1) {
            printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
            abort();
        }

        network.tcpSocket = SDLNet_TCP_Open(&network.ip);

        network.udpSocket = SDLNet_UDP_Open(CR_NET_PORT);
        if(network.udpSocket == NULL)
        {
            printf("Could not bind open socket");
            abort();
        }

        network.udpChannel = SDLNet_UDP_Bind(network.udpSocket, -1, &network.ip);
        if(network.udpChannel == -1)
        {
            printf("Could not bind UDP socket");
            abort();
        }

        network.udpSocketSet = SDLNet_AllocSocketSet(1);
        if(network.udpSocketSet == NULL)
        {
            printf("Couldn't create socket set: %s\n", SDLNet_GetError());
            abort();
        }

        int numused = SDLNet_UDP_AddSocket(network.udpSocketSet, network.udpSocket);
        if(numused == -1) {
            printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
            abort();
        }
#else
        network.tcpSocket = NULL;
#endif
    }
    
    return &network;
}

#ifdef CR_CLIENT
bool Connect(const char* host)
{
    Network* network = GetNetwork();

    // TCP
    if(SDLNet_ResolveHost(&network->ip, host, CR_NET_PORT) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    
    network->tcpSocket = SDLNet_TCP_Open(&network->ip);

    if(network->tcpSocket == 0) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return false;
    }

    // UDP
    network->udpSocket = SDLNet_UDP_Open(0);
    if(network->udpSocket == NULL)
    {
        printf("Could not open UDP socket");
        return false;
    }

    network->udpChannel = SDLNet_UDP_Bind(network->udpSocket, -1, &network->ip);
    if(network->udpChannel == -1)
    {
        printf("Could not bind UDP socket");
        return false;
    }

    return true;
}
#endif

bool SendTCPMessage(TCPsocket socket, void* content, uint16_t contentLength)
{
    // Stack alloc array of bytes
    size_t msglen = contentLength+2; // length of message + 2 bytes for uint16_t
    uint8_t* msg = alloca(msglen);

    // Interpret first 2 bytes as 16-bit unsigned int
    *((uint16_t*)(&msg[0])) = contentLength;
    // Copy contents into message starting from 3rd byte
    memcpy(&msg[2], content, contentLength);

    // Send msg through socket
    size_t sentBytes = SDLNet_TCP_Send(socket, msg, msglen);
    if(sentBytes < msglen)
    {
        // SDL_Log("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        return false;
    }

    return true;
}

bool SendTCPMessageArray(TCPsocket socket, void* items, uint16_t itemSize, uint16_t itemCount)
{
    size_t arraysize = itemSize*itemCount;

    // Stack alloc array of bytes
    size_t msglen = arraysize+4; // length of message + 2 for itemsize + 2 for itemcount
    uint8_t* msg = alloca(msglen);

    // Interpret first 2 bytes as 16-bit unsigned int
    *((uint16_t*)(&msg[0])) = itemSize;
    // Interpret second 2 bytes as 16-bit unsigned int
    *((uint16_t*)(&msg[2])) = itemCount;
    // Copy contents into message starting from 5th byte
    memcpy(&msg[4], items, arraysize);
    
    // Send msg through socket
    size_t sentBytes = SDLNet_TCP_Send(socket, msg, msglen);
    if(sentBytes < msglen)
    {
        // SDL_Log("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        return false;
    }

    return true;
}

bool SendTCPMessageNoCopy(TCPsocket socket, void* content, uint16_t contentLength)
{

    // Send msg through socket
    size_t sentBytes = SDLNet_TCP_Send(socket, content, contentLength);
    if(sentBytes < contentLength)
    {
        // SDL_Log("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        return false;
    }

    return true;
}

uint16_t GetTCPMessageLength(TCPsocket socket)
{

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

// Call GetTCPMessageLength before ReadTCPMessage!!!
bool ReadTCPMessage(TCPsocket socket, void* buffer, uint16_t len)
{

    // Read content
    if(SDLNet_TCP_Recv(socket, buffer, len) <= 0)
    {
        return false;
    }

    return true;
}

// Call GetTCPMessageLength twice before ReadTCPMessageArray!!!
bool ReadTCPMessageArray(TCPsocket socket, void* buffer, uint16_t datasize, uint16_t count)
{
    // Read content
    for (size_t i = 0; i < count; i++)
    {
        void* bufloc = (uintptr_t)buffer+(i*datasize);
        if(SDLNet_TCP_Recv(socket, bufloc, datasize) <= 0)
        {
            return false;
        }
    }
    
    return true;
}

bool SendUDPPacket(UDPpacket* packet)
{
    Network* net = GetNetwork();

    if(packet->len > CR_MAX_UDP_PACKET_SIZE)
    {
        LogInfo("SIZE OF PACKET WAS LARGER THAN %d\n", CR_MAX_UDP_PACKET_SIZE);
        abort();
        return false;
    }

    SDLNet_UDP_Send(net->udpSocket, net->udpChannel, packet);

    return true;
}
