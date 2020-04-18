#include <stdio.h>
#include <SDL_net.h>
#include <string.h>
#include <Windows.h>
#include <stdbool.h>

#include "network.h"

#define MAXLEN 1024

int main(int argc, char const *argv[])
{
	printf("Corona Royale Server\n");
	
	Network* network = GetNetwork();

	TCPsocket new_tcpsock;
    const char *text = "hello client";
	while (1)
	{
		// Sleep(1000);
		new_tcpsock = SDLNet_TCP_Accept(network->tcpsock);
		if(!new_tcpsock) {
			// printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
		}
		else {
			printf("Connection incoming\n");

			// communicate over new_tcpsock
            int len = strlen(text)+1;
			if(SDLNet_TCP_Send(new_tcpsock,text,len)<len)
            {
                printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
                abort();
            }

            int result;
            char msg[MAXLEN];

            result=SDLNet_TCP_Recv(network->tcpsock,msg,len);
            if(result<=0) {
                // An error may have occured, but sometimes you can just ignore it
                // It may be good to disconnect sock because it is likely invalid now.
                // abort();
            }
            
            printf("Received: \"%s\"\n",msg);
		}
	}

	return 0;
}
