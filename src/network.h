#pragma once
#include <SDL_net.h>

typedef struct Network
{
    TCPsocket tcpsock;
} Network;

Network* GetNetwork();

#ifdef CR_CLIENT
bool SendTCPMessage(uint8_t* content, uint16_t contentLength);
uint16_t GetTCPMessageLength();
bool ReadTCPMessage(uint8_t* buffer, uint16_t len);
#else
bool SendTCPMessage(TCPsocket* socket, uint8_t* content, uint16_t contentLength);
uint16_t GetTCPMessageLength(TCPsocket* socket);
bool ReadTCPMessage(TCPsocket* socket, uint8_t* buffer, uint16_t len);
#endif