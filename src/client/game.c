#pragma once

#include "game.h"
#include "timer.h"
#include "texture.h"
#include "menu.h"

#include <SDL_image.h>
#include <stdio.h>

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

        if (TTF_Init() < 0) { 
             abort();
        }
        // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
        game.running = true;
        game.window = SDL_CreateWindow("Corona Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
        game.renderer = SDL_CreateRenderer(game.window, -1, 0);
        CreatePlayer(&game.player,WINDOW_W/2,WINDOW_H/2);
   
        game.background = IMG_LoadTexture(game.renderer,"res/BIG.jpg");
        SDL_QueryTexture(game.background, NULL, NULL, &game.mapWidth, &game.mapHeight);
        // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
        CreateTimer(&game.timer);
        LoadMenu(&game.menu);
        
    }

    return &game;
}

void DisposeGame(){
    Game* game = GetGame();

    SDL_DestroyTexture(game->menu.textureExitButton);
    SDL_DestroyTexture(game->menu.texturePlayButton);
    SDL_DestroyTexture(game->menu.textureMenu);
    SDL_DestroyTexture(game->timer.textureTime);
    SDL_DestroyTexture(game->background);
    SDL_DestroyTexture(game->player.texture);
}
