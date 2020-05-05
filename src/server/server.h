#pragma once

#include <stdint.h>
#include "shared/hashtable.h"
#include "netplayer.h"

#define CR_MAX_PLAYERS 10

typedef struct Server {
    hashtable_t players;
} Server;
Server* GetServer();

uint16_t GetPlayerCount();

// Returns pointer to the first element in the array of all players
NetPlayer* GetAllPlayers();
NetPlayer* GetPlayer(PlayerID id);

NetPlayer* InitPlayer(TCPsocket tcpSocket, uint16_t udpPort);

// Fills dataArray and returns the count
int GetAllPlayerData(PlayerData* dataArray);

// NOT THREAD SAFE
void ApplyMovementDataToPlayer(PlayerMovementData* data);

void ServerAcceptConnection();
void ServerBroadcastPlayerData();
void ServerReadUpdates();
