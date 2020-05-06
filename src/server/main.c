#include <stdio.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <assert.h>

#define HASHTABLE_IMPLEMENTATION
#include "shared/hashtable.h"
#undef HASHTABLE_IMPLEMENTATION

#include "shared/memory.h"
#include "shared/network.h"
#include "shared/data.h"
#include "shared/log.h"
#include "shared/netevent.h"

#include "netplayer.h"
#include "server.h"

void NetworkDisconnected(TCPsocket socket)
{
    NetPlayer* player = ServerGetPlayerBySocket(socket);
    if(player == NULL)
    {
        LogInfo("UNASSIGNED PLAYER GOT DISCONNECTED\n");
        return;
    }

    PlayerID id = player->data.id;
    LogInfo("Player %d DISCONNECTED\n", id);
    ServerDisposePlayer(player);

    NetPlayer* players = ServerGetAllPlayers();
    for (size_t i = 0; i < ServerGetPlayerCount(); i++)
    {
        NetEventPlayerDisconnected data = { id };
        NetEventPlayerDisconnectedSend(players[i].tcpSocket, &data);
    }

    // abort();
}

int main(int argc, char const *argv[])
{
	printf("Corona Royale Server\n");

    assert(CR_MAX_PLAYERS <= 100, "MAXIMUM AMOUNT OF PLAYERS IS 100");

    // Seed rand
    srand(time(NULL));
	
	Network* net = GetNetwork();
    Server* server = GetServer();

	while (1)
	{
		time_t tickstart = NetworkStartTick();
        ///////// START OF NET TICK
        
        ServerReadEvents();
        ServerUpdate();
		ServerAcceptConnection();
        ServerBroadcastPlayerData();
        ServerReadUpdates();

        ///////// END OF NET TICK
        NetworkEndTick(tickstart);
    }

	return 0;
}
