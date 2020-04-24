#pragma once

#include <stdbool.h>

#include "network.h"

typedef enum DataStruct {
    CR_DATA_CUSTOM,
    CR_DATA_PLAYER,
    CR_DATA_POSITION
} DataID;

DataID GetDataID_UDP(UDPpacket* packet);


typedef struct {
    int x, y;
    float angle;
    bool infected;
} PlayerData;


typedef struct {
    int x, y;
} PositionData;

void SendPositionData_UDP(PositionData* data);
PositionData* GetPositionData_UDP(UDPpacket* packet);


// void ReadData_UDP();
