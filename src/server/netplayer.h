#pragma once

#include <stdbool.h>

#include "shared/network.h"
#include "shared/data.h"

typedef struct NetPlayer {
    TCPsocket tcpSocket;
    int x, y;
    float angle;
    bool infected;
} NetPlayer;

void InitNetPlayer(NetPlayer* player, TCPsocket tcpSocket);
void GetNetPlayerData(NetPlayer* player, PlayerData* data);