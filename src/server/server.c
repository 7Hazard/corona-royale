#include "server.h"
#include "netplayer.h"
#include "shared/memory.h"
#include "shared/log.h"

Server* GetServer()
{
    static Server server;
    static bool inited = false;
    if(!inited)
    {
        inited = true;

        hashtable_init(&server.players, sizeof(NetPlayer), CR_MAX_PLAYERS, 0);
    }

    return &server;
}

uint16_t GetPlayerCount()
{
    Server* server = GetServer();
    return hashtable_count(&server->players);
}

NetPlayer* GetAllPlayers()
{
    return hashtable_items(&GetServer()->players);
}

NetPlayer* InitPlayer(TCPsocket tcpSocket)
{
    Server* server = GetServer();

    NetPlayer player;

    player.tcpSocket = tcpSocket;
    player.data.x = rand() % 500 + 1;
    player.data.y = rand() % 500 + 1;
    player.data.angle = 0;
    player.data.infected = false;
    player.data.id = GetPlayerCount();

    return hashtable_insert(&server->players, tcpSocket, &player);
}

int GetAllPlayerData(PlayerData* dataArray)
{
    Server* server = GetServer();

    int count = hashtable_count(&server->players);
    // HASHTABLE_U64 const* keys = hashtable_keys( &server->players );
    NetPlayer* players = hashtable_items(&server->players);
    for (size_t i = 0; i < count; i++)
    {
        NetPlayer* otherply = &players[i];

        memcpy(&dataArray[i], &otherply->data, sizeof(PlayerData));
    }

    return count;
}
