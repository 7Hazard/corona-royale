#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include "events.h"

int running = 1;
int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    if(!SDL_Init(0))
    {
      printf("Could not initialize SDL2");
    }

    SDL_ShowSimpleMessageBox(0, "Corona Royale", "It's corona time!", NULL);
    SDL_Window* window = SDL_CreateWindow("Corona Royale",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,1000,0);
    while (running)
    {
      HandleEvents();
    }

    SDL_Quit();
    return 0;
}

