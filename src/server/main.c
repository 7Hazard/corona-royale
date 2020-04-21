#include <stdio.h>
#include <SDL_net.h>
#include <string.h>
#include <stdbool.h>

#include "memory.h"
#include "network.h"

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
            
		}
	}

	return 0;
}
