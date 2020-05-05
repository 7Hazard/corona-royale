#pragma once

#include "network.h"
#include "data.h"

typedef enum NetEvent {
    CR_NETEVENT_NONE,
    CR_NETEVENT_DISCONNECTED,
    CR_NETEVENT_PLAYER_CONNECTED
} NetEvent;

typedef struct PlayerConnectedEvent {
    PlayerData data;
} PlayerConnectedEvent;

/** Events should be polled in a separate thread
 * TCP, BLOCKING FUNCTION
 * returns the NetEvent recieved or CR_NETEVENT_DISCONNECTED if disconnected
 * */
NetEvent GetNetEvent();

bool ReadPlayerConnectedEvent(TCPsocket socket, PlayerConnectedEvent* destination);

bool SendPlayerConnectedEvent(TCPsocket socket, PlayerConnectedEvent* data);
