#include <stdio.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>

#include "memory.h"
#include "network.h"
#include "data.h"

#define MAXLEN 1024

int main(int argc, char const *argv[])
{
	printf("Corona Royale Server\n");
	
	Network* network = GetNetwork();

	TCPsocket clientSock;
	const char* text = "hello client";

	while (1)
	{
		clientSock = SDLNet_TCP_Accept(network->tcpsock);
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
	}

	return 0;
}
