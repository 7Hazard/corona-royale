#pragma once

#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

#include "shared/log.h"

#include "game.h"
#include "timer.h"
#include "textures.h"


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
        game.connected = false;
        game.window = SDL_CreateWindow("Corona Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
        game.renderer = SDL_CreateRenderer(game.window, -1, 0);

        Textures* tex = GetTextures(); // NEEDS TO BE CALLED AFTER RENDERER IS CREATED
        game.background = tex->grass;
        SDL_QueryTexture(game.background, NULL, NULL, &game.mapWidth, &game.mapHeight);
        CreatePlayer(&game.player, 10, 10);
        CreateTimer(&game.timer);

        // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
    }

    return &game;
}

void GameInitNetPlayersTable(uint16_t count)
{
    Game* game = GetGame();
    hashtable_init(&game->players, sizeof(NetPlayer), count, 0);
}

void GameInitNetPlayers(PlayerData* players, uint16_t count)
{
    Game* game = GetGame();

    GameInitNetPlayersTable(count);
    for (size_t i = 0; i < count; i++)
    {
        NetPlayer player;
        InitNetPlayer(&player, &players[i]);

        // debug
        LogInfo("ID: %d | x: %d | y: %d | angle: %f | inf: %d", player.data.id, player.data.x, player.data.y, player.data.angle, player.data.infected);
        // debug
        
        hashtable_insert(&game->players, player.data.id, &player);
    }
}

void GameInitNetPlayer(PlayerData* data)
{
    Game* game = GetGame();

    NetPlayer player;
    InitNetPlayer(&player, data);

    // debug
    LogInfo("ID: %d | x: %d | y: %d | angle: %f | inf: %d", player.data.id, player.data.x, player.data.y, player.data.angle, player.data.infected);
    // debug

    hashtable_insert(&game->players, player.data.id, &player);
}

void GameDisposeNetPlayers()
{
    Game* game = GetGame();

    NetPlayer* players = GetAllPlayers();
    for (size_t i = 0; i < GetPlayerCount(); i++)
    {
        DisposeNetPlayer(&players[i]);
    }

    hashtable_term(&game->players);
}

uint16_t GetPlayerCount()
{
    return hashtable_count(&GetGame()->players);
}

NetPlayer* GetAllPlayers()
{
    return hashtable_items(&GetGame()->players);
}

NetPlayer* GetPlayer(PlayerID id)
{
    Game* game = GetGame();
    return hashtable_find(&game->players, id);
}

void ApplyMovementDataToPlayer(PlayerMovementData* data)
{
    NetPlayer* player = GetPlayer(data->id);

    SDL_LockMutex(player->mutex);
    
    player->data.angle = data->angle;
    player->data.x = data->x;
    player->data.y = data->y;

    SDL_UnlockMutex(player->mutex);
}
