#pragma once

#include <stdbool.h>

#include "network.h"

typedef enum DataStruct {
    CR_DATA_CUSTOM,
    CR_DATA_PLAYER,
    CR_DATA_POSITION
} DataID;

typedef uint16_t PlayerID;

DataID GetDataID_UDP(UDPpacket* packet);

typedef struct PlayerData {
    PlayerID id;
    int x, y;
    float angle;
    bool infected;
} PlayerData;


typedef struct PlayerPositionData {
    PlayerID id;
    int x, y;
} PlayerPositionData;

void SendPositionData_UDP(PlayerPositionData* data);
PlayerPositionData* GetPositionData_UDP(UDPpacket* packet);


// void ReadData_UDP();
