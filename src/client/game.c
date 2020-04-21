#pragma once

#include "game.h"
#include "timer.h"
#include "texture.h"
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

Game* GetGame()
{
    // Will initialize after first call
    static bool inited = false;
    static Game game;
    if(inited == false)
    {
        inited = true;

        if(SDL_Init(SDL_INIT_AUDIO)!=0) 
        {
            SDL_Log("SDL_Init: %s\n", SDL_GetError());
            abort();
            exit(0);
        }
        int flags = IMG_INIT_JPG|IMG_INIT_PNG;
        if((IMG_Init(flags) & flags) != flags) 
        {
            printf("IMG_Init: Failed to init required jpg and png support!\n");
            printf("IMG_Init: %s\n", IMG_GetError());
            abort();
            exit(3);
        }

        // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
        game.running = true;
        game.window = SDL_CreateWindow("Corona Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
        game.renderer = SDL_CreateRenderer(game.window, -1, 0);
        CreatePlayer(&game.player,10,10);
        game.background = LoadTexture("res/BIG.jpg");
        SDL_QueryTexture(game.background, NULL, NULL, &game.mapWidth, &game.mapHeight);
        // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
        CreateTimer(&game.timer);
        
    }

    return &game;
}
