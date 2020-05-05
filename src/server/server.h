#pragma once

#include <stdint.h>

#include "netplayer.h"

#include "shared/hashtable.h"

#define CR_MAX_PLAYERS 10
#define CR_MAP_WIDTH 2149
#define CR_MAP_HEIGHT 2149

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

void ServerDisconnectPlayer(NetPlayer* player);

// NOT THREAD SAFE
void ApplyMovementDataToPlayer(PlayerMovementData* data);

void ServerUpdate();
void ServerAcceptConnection();
void ServerBroadcastPlayerData();
void ServerReadUpdates();
