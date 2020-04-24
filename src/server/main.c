#include <stdio.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <assert.h>

#include "shared/memory.h"
#include "shared/network.h"
#include "shared/data.h"
#include "netplayer.h"
#include "shared/log.h"

#define CR_MAX_PLAYERS 10

int main(int argc, char const *argv[])
{
	printf("Corona Royale Server\n");

    assert(CR_MAX_PLAYERS <= 100, "MAXIMUM AMOUNT OF PLAYERS IS 100");

    // Seed rand
    srand(time(NULL));
	
	Network* net = GetNetwork();

    // Allocate array of net players
    NetPlayer* players = alloca(CR_MAX_PLAYERS*sizeof(NetPlayer));
    int playerCount = 0;

	TCPsocket incomingSocket;

    UDPpacket* packet = SDLNet_AllocPacket(1024);

	while (1)
	{
		time_t start = clock();
        ///////// START OF NET TICK
        
		incomingSocket = SDLNet_TCP_Accept(net->tcpSocket);
		if(incomingSocket)
        {
			printf("Connection incoming\n");

            uint16_t len = GetTCPMessageLength(incomingSocket);
            if(len == 0)
            {
                printf("Could not get length of TCP message, connection invalid!\n");
                continue;
            }
            printf("TCP Message length: %d\n", len);

            // Stack allocate enough memory for content
            char* content = alloca(len);
            if(!ReadTCPMessage(incomingSocket, content, len))
            {
                printf("COULD NOT READ MESSAGE");
                continue;
            }
            
            printf("Recieved: %s\n", content);

            if(playerCount == CR_MAX_PLAYERS)
            {
                LogInfo("MAXIMUM PLAYERS REACHED, REFUSING CONNECTION\n");
                // TODO: SEND ACTUAL REASON FOR REFUSED CONNECTION
                SDLNet_TCP_Close(incomingSocket);
            }
            else {
                // Register player
                NetPlayer* ply = &players[playerCount];
                InitNetPlayer(ply, incomingSocket);

                // Send player data
                PlayerData data;
                GetNetPlayerData(ply, &data);
                SendTCPMessage(incomingSocket, &data, sizeof(data));

                LogInfo("Registered new player\n");
            }
		}

        if(SDLNet_CheckSockets(net->udpSocketSet, 0))
        {
            while(SDLNet_UDP_Recv(net->udpSocket, packet))
            {
                if(GetDataID_UDP(packet) == CR_DATA_POSITION)
                {
                    PositionData* data = GetPositionData_UDP(packet);
                    printf("RECIEVED POSITION {x: %d, y: %d}\n", data->x, data->y);
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
