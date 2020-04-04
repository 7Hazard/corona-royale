#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_mixer.h>

#include "game.h"
#include "collison.h"
#include "events.h"
#include "player.h"
#include "audio.h"

int main(int argc, const char *argv[])
{
    SDL_Log("Corona Royale\n");

    const int fps = 60;
    const int frameDelay = 1000/fps;
    Uint32 frameStart;
    int frameTime;

    Game* game = GetGame();
    Audio* audio = GetAudio();

    Player player;
    CreatePlayer(&player);
    player.rect.x = 200;
    player.rect.y = 200;
    player.rect.w = 80;
    player.rect.h = 80;
    player.infected = true;

    Player player2;
    CreatePlayer(&player2);
    player2.rect.x = 300;
    player2.rect.y = 300;
    player2.rect.w = 80;
    player2.rect.h = 80;

    while (game->running)
    {
        frameStart = SDL_GetTicks();

        HandleEvents();
        HandleBorders(&player.rect);
        HandleBorders(&player2.rect);
        OnPlayerUpdate(&player);
        
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);
        
        if(DoBoxesIntersect(&player.rect, &player2.rect) && player2.infected != true)
        {
            Mix_PlayChannel(-1, audio->cough, 0);
            player2.infected = true;
        }

        OnPlayerRender(&player);
        OnPlayerRender(&player2);
        
        SDL_RenderPresent(game->renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay >frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    
    StopAudio();
    SDL_Quit();
    return 0;
}

