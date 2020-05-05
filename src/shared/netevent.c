#include "netevent.h"

NetEvent NetEventGet()
{
    Network *net = GetNetwork();

    uint16_t event = 0;
    if (SDLNet_TCP_Recv(net->tcpSocket, &event, 2) <= 0)
    {
        // Failed to read
        // TCP socket invalid now
        // Disconnect
        return CR_NETEVENT_PlayerDisconnected;
    }
    else
        return (NetEvent)event;
}
