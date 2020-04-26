#pragma once

#include <stdbool.h>

#include "shared/network.h"
#include "shared/data.h"

typedef struct NetPlayer {
    TCPsocket tcpSocket;
    PlayerData data;
} NetPlayer;
