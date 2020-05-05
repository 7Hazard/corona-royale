#pragma once

#include "network.h"
#include "data.h"

typedef enum NetEvent {
    CR_NETEVENT_None,
    CR_NETEVENT_Disconnected,
    CR_NETEVENT_PlayerConnected,
    CR_NETEVENT_PlayerDisconnected,
    CR_NETEVENT_PlayerInfected
} NetEvent;

/** Events should be polled in a separate thread
 * TCP, BLOCKING FUNCTION
 * returns the NetEvent recieved or CR_NETEVENT_DISCONNECTED if disconnected
 * */
NetEvent NetEventGet();

#define CR_NETEVENT_IMPL_READ(event) \
inline bool NetEvent##event##Read(TCPsocket socket, NetEvent##event *destination) { \
    if (!ReadTCPMessage(socket, destination, sizeof(NetEvent##event))) \
        return false; \
    return true; \
}
#define CR_NETEVENT_IMPL_SEND(event) \
inline bool NetEvent##event##Send(TCPsocket socket, NetEvent##event *data) { \
    uint16_t id = CR_NETEVENT_##event; \
    if (!SendTCPMessageNoCopy(socket, &id, sizeof(id)) || !SendTCPMessageNoCopy(socket, data, sizeof(NetEvent##event))) \
        return false; \
    return true; \
}
#define CR_NETEVENT_STRUCT(event, fields) typedef struct NetEvent##event fields NetEvent##event;
#define CR_NETEVENT(event, fields) CR_NETEVENT_STRUCT(event, fields) CR_NETEVENT_IMPL_READ(event) CR_NETEVENT_IMPL_SEND(event)

CR_NETEVENT(PlayerConnected, {
    PlayerData data;
})
CR_NETEVENT(PlayerDisconnected, {
    PlayerID id;
})
CR_NETEVENT(PlayerInfected, {
    PlayerID id;
})
