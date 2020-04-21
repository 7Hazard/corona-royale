#pragma once
#include <SDL_net.h>

typedef struct Network
{
    TCPsocket tcpsock;
} Network;

Network* GetNetwork();

#ifdef CR_CLIENT // CLIENT FUNCTIONS

bool ConnectTCP(const char* host);
// Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
bool SendTCPMessage(uint8_t* content, uint16_t contentLength);
// Will wait until a message is incoming
// If returns 0, SOCKET IS INVALIDATED THEN
uint16_t GetTCPMessageLength();
// Returns false if could not read message, CONNECTION IS INVALIDATED NOW!!
bool ReadTCPMessage(uint8_t* buffer, uint16_t len);

#else // SERVER FUNCTIONS

// Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
bool SendTCPMessage(TCPsocket* socket, uint8_t* content, uint16_t contentLength);
// Will wait until a message is incoming
// If returns 0, SOCKET IS INVALIDATED THEN
uint16_t GetTCPMessageLength(TCPsocket* socket);
// Returns false if could not read message, SOCKET IS INVALIDATED NOW!!
bool ReadTCPMessage(TCPsocket* socket, uint8_t* buffer, uint16_t len);

#endif