#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

int main(int argc, const char *argv[])
{
  printf("Corona Royale\n");

  if(!SDL_Init(0))
  {
    printf("Could not initialize SDL2");
  }

  SDL_ShowSimpleMessageBox(0, "Corona Royale", "It's corona time!", NULL);

  // while (true)
  // {
    
  // }

  SDL_Quit();
  return 0;
}

