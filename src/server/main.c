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
    const char *text = "hello client";
	while (1)
	{
		// Sleep(1000);
		clientSock = SDLNet_TCP_Accept(network->tcpsock);
		if(!clientSock) {
			// printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
		}
		else {
			printf("Connection incoming\n");

			// communicate over new_tcpsock
            // int len = strlen(text)+1;
			// printf("%d\n\n",len);
			// if(SDLNet_TCP_Send(new_tcpsock,text,len)<len)
			// {
			//     printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			//     abort();
			// }

            uint16_t len = GetTCPMessageLength(clientSock);
            printf("TCP Message length: %d\n", len);

            // Stack allocate enough memory for content
            char* content = alloca(len);
            if(!ReadTCPMessage(clientSock, content, len))
            {
                printf("COULD NOT READ MESSAGE");
            }
            else {
                printf("Recieved: %s\n", content);
            }
		}
	}

	return 0;
}
