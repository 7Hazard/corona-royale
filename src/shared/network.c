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
        IPaddress tcpaddr;
        if(SDLNet_ResolveHost(&tcpaddr, NULL, CR_NET_PORT) == -1) {
            printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
            abort();
        }

        network.tcpSocket = SDLNet_TCP_Open(&tcpaddr);

        // UDP
        IPaddress udpaddr = tcpaddr;
        udpaddr.port = CR_NET_PORT;
        network.udpSocket = SDLNet_UDP_Open(CR_NET_PORT);
        if(network.udpSocket == NULL)
        {
            printf("Could not bind open socket");
            abort();
        }

        // network.udpChannel = SDLNet_UDP_Bind(network.udpSocket, 1, &udpaddr);
        // if(network.udpChannel == -1)
        // {
        //     printf("Could not bind UDP socket");
        //     abort();
        // }

        network.udpSocketSet = SDLNet_AllocSocketSet(2);
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
    if(SDLNet_ResolveHost(&network->address, host, CR_NET_PORT) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    
    network->tcpSocket = SDLNet_TCP_Open(&network->address);

    if(network->tcpSocket == 0) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return false;
    }

    // UDP
    network->udpSocket = SDLNet_UDP_Open(0);
    if(network->udpSocket == NULL)
    {
        printf("Could not open UDP socket\n");
        return false;
    }

    // Send client UDP port for server to talk to
    uint16_t port = SDLNet_UDP_GetPeerAddress(network->udpSocket, -1)->port;
    if(!SendTCPMessage(network->tcpSocket, &port, sizeof(uint16_t)))
    {
        LogInfo("Could not send UDP port to server");
        abort();
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

bool SendTCPMessageArray(TCPsocket socket, void* items, uint16_t itemSize, uint16_t itemsCount)
{
    size_t arraysize = itemSize*itemsCount;

    // Send item size
    SendTCPMessageNoCopy(socket, &itemSize, sizeof(uint16_t));
    // Send items count
    SendTCPMessageNoCopy(socket, &itemsCount, sizeof(uint16_t));
    
    // Send msg through socket
    for (size_t i = 0; i < itemsCount; i++)
    {
        void* item = (uintptr_t)items+(i*itemSize);
        size_t sentBytes = SDLNet_TCP_Send(socket, item, itemSize);
        if(sentBytes < itemSize)
        {
            // SDL_Log("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            return false;
        }
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

    int result = SDLNet_UDP_Send(net->udpSocket, -1, packet);
    if(!result)
    {
        return false;
    }

    return true;
}

time_t NetworkStartTick()
{
    return clock();
}

void NetworkEndTick(time_t tickstart)
{
    time_t end = clock();
    int result = end-tickstart;
    if (result < CR_NET_TICK_TIME)
    {
        Sleep(CR_NET_TICK_TIME-result);
    }
}