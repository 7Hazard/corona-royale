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

// Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
bool SendTCPMessage(uint8_t* content, uint16_t contentLength);

// Will wait until a message is incoming
// If returns 0, SOCKET IS INVALIDATED THEN
uint16_t GetTCPMessageLength();

// Returns false if could not read message, CONNECTION IS INVALIDATED NOW!!
bool ReadTCPMessage(uint8_t* buffer, uint16_t len);

void SendUDPPacket(UDPpacket* packet);


#else
// SERVER FUNCTIONS

// Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
bool SendTCPMessage(TCPsocket socket, uint8_t* content, uint16_t contentLength);

// Will wait until a message is incoming
// If returns 0, SOCKET IS INVALIDATED THEN
uint16_t GetTCPMessageLength(TCPsocket socket);

// Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
bool ReadTCPMessage(TCPsocket socket, uint8_t* buffer, uint16_t len);


#endif
// Universal functions

// void PollUDPUpdates();
