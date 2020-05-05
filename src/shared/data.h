#pragma once

#include <stdbool.h>

#include "network.h"

typedef enum DataStruct {
    CR_DATA_CUSTOM,
    CR_DATA_PLAYER,
    CR_DATA_MOVEMENT
} DataID;

typedef uint16_t PlayerID;

DataID GetDataID_UDP(UDPpacket* packet);

typedef struct PlayerData {
    PlayerID id;
    int x, y;
    float angle;
    bool infected;
} PlayerData;


typedef struct PlayerMovementData {
    PlayerID id;
    int x, y;
    float angle;
} PlayerMovementData;

#ifdef CR_CLIENT
void SendMovementData_UDP(PlayerMovementData* data);
#else
void SendMovementData_UDP(IPaddress* addr, PlayerMovementData* data);
#endif
PlayerMovementData* GetMovementData_UDP(UDPpacket* packet);


// void ReadData_UDP();
