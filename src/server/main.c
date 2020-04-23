#include <stdio.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>


#include "shared/memory.h"
#include "shared/network.h"
#include "shared/data.h"

void HandlePacket(UDPpacket* packet)
{
   static int count = 0;
    printf("UDP PACKET RECIEVED %d\n",count++);
}

int main(int argc, char const *argv[])
{
	printf("Corona Royale Server\n");
	
	Network* network = GetNetwork();
    network->udpPacketRecieveCallback = HandlePacket;

	TCPsocket clientSock;
	const char* text = "hello client";

	while (1)
	{
		time_t start = clock();
        
        clientSock = SDLNet_TCP_Accept(network->tcpSocket);
		if(clientSock)
        {
			printf("Connection incoming\n");

            uint16_t len = GetTCPMessageLength(clientSock);
            if(len == 0)
            {
                printf("Could not get length of TCP message, connection invalid!");
                continue;
            }
            printf("TCP Message length: %d\n", len);

            // Stack allocate enough memory for content
            char* content = alloca(len);
            if(!ReadTCPMessage(clientSock, content, len))
            {
                printf("COULD NOT READ MESSAGE");
                continue;
            }
            
            printf("Recieved: %s\n", content);

            // SEND START INFO TO PLAYER
            {
                srand(time(NULL));
                PlayerData data;
                data.x = rand() % 500 + 1;
                data.y = rand() % 500 + 1;
            
                SendTCPMessage(clientSock, &data, sizeof(data));
            }
		}

        CheckUDPUpdates();

        time_t end = clock();
        int result = end-start;
        if (result < CR_NET_TICK_TIME)
        {
            Sleep(CR_NET_TICK_TIME-result);
        }
	}

	return 0;
}
