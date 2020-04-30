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
    UDPpacket* recvpacket = SDLNet_AllocPacket(1024);

	while (1)
	{
		time_t start = clock();
        ///////// START OF NET TICK
        
		incomingSocket = SDLNet_TCP_Accept(net->tcpSocket);
		if(incomingSocket)
        {
            // Get client UDP port to send packets to
            if(GetTCPMessageLength(incomingSocket) == 0)
            {
                printf("Could not get client UDP port, connection invalid!\n");
                continue;
            }
            uint16_t udpPort = 0;
            if(!ReadTCPMessage(incomingSocket, &udpPort, sizeof(uint16_t)))
            {
                printf("COULD NOT READ CLIENT PORT\n");
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
                NetPlayer* connectingplayer = InitPlayer(incomingSocket, udpPort);
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

        { // Send data to players
            NetPlayer* players = GetAllPlayers();
            uint16_t playercount = GetPlayerCount();
            // PlayerData* data = alloca(sizeof(PlayerData)+playercount);
            // int datacount = GetAllPlayerData(&data);

            // for every player
            for (size_t i = 0; i < playercount; i++)
            {
                // Get the player
                NetPlayer* player = &players[i];

                // send all others players data to the player
                for (size_t i = 0; i < playercount; i++)
                {
                    // get the other player
                    NetPlayer* otherPlayer = &players[i];

                    // don't send player data to himself
                    if(otherPlayer == player) continue;

                    // get other players data
                    PlayerMovementData data;
                    NetPlayerGetMovementData(otherPlayer, &data);
                    // get recipient udp ip
                    IPaddress udpaddr;
                    udpaddr.host = NetPlayerGetTCPAddress(player)->host;
                    udpaddr.port = player->udpPort;
                    // send other player's data to the recipient
                    SendMovementData_UDP(&udpaddr, &data);
                }
            }
        }
        
        // Read updates from players
        if(SDLNet_CheckSockets(net->udpSocketSet, 0))
        {
            while(SDLNet_UDP_Recv(net->udpSocket, recvpacket))
            {
                DataID dataid = GetDataID_UDP(recvpacket);
                switch (dataid)
                {
                case CR_DATA_MOVEMENT:
                {
                    PlayerMovementData* data = GetMovementData_UDP(recvpacket);
                    // printf("RECIEVED POSITION {x: %d, y: %d}\n", data->x, data->y);
                    ApplyMovementDataToPlayer(data);
                    break;
                }
                
                default:
                    LogInfo("UNHANDLE DATA ID %d", dataid);
                    break;
                }

                
            }
        }

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
