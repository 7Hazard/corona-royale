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

int main(int argc, char const *argv[])
{
	printf("Corona Royale Server\n");

    assert(CR_MAX_PLAYERS <= 100, "MAXIMUM AMOUNT OF PLAYERS IS 100");

    // Seed rand
    srand(time(NULL));
	
	Network* net = GetNetwork();
    Server* server = GetServer();

	TCPsocket incomingSocket;
    UDPpacket* packet = SDLNet_AllocPacket(1024);

	while (1)
	{
		time_t start = clock();
        ///////// START OF NET TICK
        
		incomingSocket = SDLNet_TCP_Accept(net->tcpSocket);
		if(incomingSocket)
        {
            uint16_t len = GetTCPMessageLength(incomingSocket);
            if(len == 0)
            {
                printf("Could not get length of TCP message, connection invalid!\n");
                continue;
            }

            // Stack allocate enough memory for content
            char* content = alloca(len);
            if(!ReadTCPMessage(incomingSocket, content, len))
            {
                printf("COULD NOT READ MESSAGE");
                continue;
            }
            
            // Player count before connecting
            uint16_t playercount = GetPlayerCount();
            if(playercount == CR_MAX_PLAYERS)
            {
                LogInfo("MAXIMUM PLAYERS REACHED, REFUSING CONNECTION\n");
                char* reason = "SERVER IS FULL";
                SendTCPMessage(incomingSocket, reason, strlen(reason)+1);
                SDLNet_TCP_Close(incomingSocket);
            }
            else {
                // Send ok confirmation
                char* reason = "OK";
                SendTCPMessage(incomingSocket, reason, strlen(reason)+1);

                // Send max players
                {
                    uint16_t maxplayers = CR_MAX_PLAYERS;
                    SendTCPMessageNoCopy(incomingSocket, &maxplayers, sizeof(uint16_t));
                }

                // Send all player data to player
                PlayerData* data = alloca(sizeof(PlayerData)+playercount);
                int playercount = GetAllPlayerData(data);
                if(!SendTCPMessageArray(incomingSocket, data, sizeof(PlayerData), playercount))
                {
                    LogInfo("COULD NOT SEND ALL PLAYER DATA");
                    abort();
                }

                // Register and send player
                NetPlayer* connectingplayer = InitPlayer(incomingSocket);
                if(!SendTCPMessage(incomingSocket, &connectingplayer->data, sizeof(PlayerData)))
                {
                    LogInfo("COULD NOT SEND PLAYER DATA");
                    abort();
                }

                // Send connecting player event to all players
                NetPlayer* players = GetAllPlayers();
                for (size_t i = 0; i < playercount; i++)
                {
                    NetPlayer* player = &players[i];

                    // Skip sending event to the connecting player himself
                    if(player->tcpSocket == connectingplayer->tcpSocket)
                        continue;

                    // Send event
                    PlayerConnectedEvent ev;
                    ev.data = connectingplayer->data;
                    SendPlayerConnectedEvent(player->tcpSocket, &ev);
                }

                LogInfo("Player connected, ID: %d\n", connectingplayer->data.id);
            }
		}

        if(SDLNet_CheckSockets(net->udpSocketSet, 0))
        {
            while(SDLNet_UDP_Recv(net->udpSocket, packet))
            {
                if(GetDataID_UDP(packet) == CR_DATA_POSITION)
                {
                    PlayerPositionData* data = GetPositionData_UDP(packet);
                    // printf("RECIEVED POSITION {x: %d, y: %d}\n", data->x, data->y);
                }
            }
        }

        // Send position data to all players
        

        ///////// END OF NET TICK
        time_t end = clock();
        int result = end-start;
        if (result < CR_NET_TICK_TIME)
        {
            Sleep(CR_NET_TICK_TIME-result);
	    }
    }

	return 0;
}
