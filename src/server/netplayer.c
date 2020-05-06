#include "netplayer.h"

#include "server.h"

#include "shared/math.h"
#include "shared/netevent.h"
#include "shared/log.h"

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

void NetPlayerUpdate(NetPlayer* player)
{
    NetPlayer* players = ServerGetAllPlayers();
    uint16_t playercount = ServerGetPlayerCount();

    // Check if player can infect someone
    for (size_t i = 0; i < playercount; i++)
    {
        // Get the player
        NetPlayer* otherply = &players[i];

        // skip if player himself
        if(player == otherply) continue;

        double dist = distance(player->data.x, player->data.y, otherply->data.x, otherply->data.y);

        // check if can infect
        if(player->data.infected && !otherply->data.infected && dist < player->data.infectionRadius)
        {
            LogInfo("PLAYER %d INFECTED %d\n", player->data.id, otherply->data.id);
            otherply->data.infected = true;

            NetEventPlayerInfected data = { otherply->data.id };

            // broadcast event
            for (size_t i = 0; i < playercount; i++)
            {
                // Get the player
                NetPlayer* ply = &players[i];

                NetEventPlayerInfectedSend(ply->tcpSocket, &data);
            }
        }
    }
}
