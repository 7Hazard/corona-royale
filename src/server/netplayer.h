#pragma once

#include <stdbool.h>

#include "shared/network.h"
#include "shared/data.h"

typedef struct NetPlayer {
    TCPsocket tcpSocket;
    // UDPsocket udpSocket;
    PlayerData data;
    uint16_t udpPort;
} NetPlayer;

IPaddress* NetPlayerGetTCPAddress(NetPlayer* player);

// NOT THREAD SAFE
void NetPlayerGetMovementData(NetPlayer* player, PlayerMovementData* data);

void NetPlayerUpdateInfectionStatus();
