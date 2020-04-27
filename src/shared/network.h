#pragma once
#include <stdbool.h>
#include <SDL_net.h>

#define CR_NET_PORT 6969
#define CR_MAX_UDP_PACKET_SIZE 256
#define CR_NET_TICK 20
#define CR_NET_TICK_TIME (1000/CR_NET_TICK)

typedef struct Network
{
    IPaddress ip;

    // TCP
    TCPsocket tcpSocket;

    // UDP
    UDPsocket udpSocket;
#ifdef CR_SERVER
    SDLNet_SocketSet udpSocketSet;
#endif
    int udpChannel;
} Network;

Network* GetNetwork();

#ifdef CR_CLIENT
// CLIENT FUNCTIONS

bool Connect(const char* host);


#else
// SERVER FUNCTIONS


#endif
// Universal functions

// Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
bool SendTCPMessage(TCPsocket socket, void* content, uint16_t contentLength);

/** Sending an array of items over TCP
 * Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
 * */
bool SendTCPMessageArray(TCPsocket socket, void* items, uint16_t itemSize, uint16_t itemCount);

/** Sends without copying or length
 * Returns false if could not send message, SOCKET IS INVALIDATED NOW!!
 * */
bool SendTCPMessageNoCopy(TCPsocket socket, void* content, uint16_t contentLength);

// Will wait until a message is incoming
// If returns 0, SOCKET IS INVALIDATED THEN
uint16_t GetTCPMessageLength(TCPsocket socket);

// Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
bool ReadTCPMessage(TCPsocket socket, void* buffer, uint16_t len);

bool ReadTCPMessageArray(TCPsocket socket, void* buffer, uint16_t datasize, uint16_t count);

bool SendUDPPacket(UDPpacket* packet);

// void PollUDPUpdates();
