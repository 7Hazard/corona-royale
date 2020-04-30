#include "netplayer.h"

// void InitNetPlayer(NetPlayer* player)
// {

// }

// void GetNetPlayerPositionData(NetPlayer* player, PlayerMovementData* data)
// {
    
// }

IPaddress* NetPlayerGetTCPAddress(NetPlayer* player)
{
    return SDLNet_TCP_GetPeerAddress(player->tcpSocket);
}

void NetPlayerGetMovementData(NetPlayer* player, PlayerMovementData* data)
{
    data->id = player->data.id;
    data->angle = player->data.angle;
    data->x = player->data.x;
    data->y = player->data.y;
}
