#include "netplayer.h"

void InitNetPlayer(NetPlayer* player, TCPsocket tcpSocket)
{
    player->tcpSocket = tcpSocket;
    player->x = rand() % 500 + 1;
    player->y = rand() % 500 + 1;
    player->angle = 0;
    player->infected = false;
}

void GetNetPlayerData(NetPlayer* player, PlayerData* data)
{
    data->x = player->x;
    data->y = player->y;
    data->angle = player->angle;
    data->infected = player->infected;
}
