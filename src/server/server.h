#pragma once

#include <stdint.h>

#include "netplayer.h"

#include "shared/hashtable.h"

#define CR_MAX_PLAYERS 10
#define CR_MAP_WIDTH 2149
#define CR_MAP_HEIGHT 2149

typedef struct Server {
    hashtable_t playersByID;
    // hashtable_t playersBySocket;
    
    SDLNet_SocketSet udpSocketSet;
    SDLNet_SocketSet tcpSocketSet;

    bool gameEnded;
    time_t gameEndTime;
} Server;
Server* GetServer();

uint16_t ServerGetPlayerCount();

// Returns pointer to the first element in the array of all players
NetPlayer* ServerGetAllPlayers();
NetPlayer* ServerGetPlayerByID(PlayerID id);
NetPlayer* ServerGetPlayerBySocket(TCPsocket socket);

NetPlayer* ServerInitPlayer(TCPsocket tcpSocket, uint16_t udpPort);
void ServerDisposePlayer(NetPlayer* player);

// Fills dataArray and returns the count
int ServerGetAllPlayerData(PlayerData* dataArray);

void ServerDisconnectPlayer(NetPlayer* player);

// NOT THREAD SAFE
void ApplyMovementDataToPlayer(PlayerMovementData* data);

void ServerUpdate();
void ServerAcceptConnection();
void ServerBroadcastPlayerData();
void ServerReadUpdates();
