#include "netevent.h"

NetEvent GetNetEvent()
{
    Network* net = GetNetwork();

    uint16_t event = 0;
    if(SDLNet_TCP_Recv(net->tcpSocket, &event, 2) <= 0)
    {
        // Failed to read
        // TCP socket invalid now
        // Disconnect
        return CR_NETEVENT_DISCONNECTED;
    }
    else return (NetEvent)event;
}

bool ReadPlayerConnectedEvent(TCPsocket socket, PlayerConnectedEvent* destination)
{
    ReadTCPMessage(socket, destination, sizeof(PlayerConnectedEvent));
}

bool SendPlayerConnectedEvent(TCPsocket socket, PlayerConnectedEvent* data)
{
    uint16_t id = CR_NETEVENT_PLAYER_CONNECTED;
    SendTCPMessageNoCopy(socket, &id, sizeof(id));
    SendTCPMessageNoCopy(socket, data, sizeof(PlayerConnectedEvent));
}
