#include "netevent.h"

NetEvent NetEventGet(TCPsocket socket)
{
    uint16_t event = 0;
    if (SDLNet_TCP_Recv(socket, &event, 2) <= 0)
    {
        // Failed to read
        // TCP socket invalid now
        // Disconnect
        NetworkDisconnected(socket);
        return CR_NETEVENT_Disconnected;
    }
    else
        return (NetEvent)event;
}
