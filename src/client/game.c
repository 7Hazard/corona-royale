#pragma once

#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "game.h"
#include "timer.h"
#include "textures.h"
#include "gamenet.h"
#include "game.h"
#include "timer.h"
#include "menu.h"
#include "audio.h"

#include "shared/log.h"
#include "shared/netevent.h"

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

        game.stateMutex = SDL_CreateMutex();
        
        game.window = SDL_CreateWindow("Corona Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CR_WINDOW_W, CR_WINDOW_H, 0);
        game.renderer = SDL_CreateRenderer(game.window, -1, 0);
        SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
        game.fps = 60; // default target FPS is 60

        Textures* tex = GetTextures(); // NEEDS TO BE CALLED AFTER RENDERER IS CREATED
        game.background = tex->grass;
        SDL_QueryTexture(game.background, NULL, NULL, &game.mapWidth, &game.mapHeight);
        CreatePlayer(&game.player, 10, 10);
        CreateTimer(&game.timer);

        // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
    }

    return &game;
}

void DisposeGame()
{
    Game* game = GetGame();
    Textures* tex = GetTextures();

    //SDL_DestroyTexture(game->menu.textureMenu);
    SDL_DestroyTexture(game->background);
    SDL_DestroyTexture(game->player.texture);
    SDL_StopTextInput();
}

Uint32 GameStartFrame()
{
    return SDL_GetTicks();
}

void GameEndFrame(Uint32 frameStart)
{
    Game* game = GetGame();

    Uint32 frameDelay = 1000/game->fps;
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < frameDelay)
    {
        SDL_Delay(frameDelay - frameTime);
    }
}

GameState GameGetState()
{
    Game* game = GetGame();
    SDL_LockMutex(game->stateMutex);
    GameState state = game->state;
    SDL_UnlockMutex(game->stateMutex);
    return state;
}

void GameSetState(GameState state)
{
    Game* game = GetGame();
    SDL_LockMutex(game->stateMutex);
    game->state = state;
    SDL_UnlockMutex(game->stateMutex);

    LogInfo("Switched to state %d", state);

    switch (state)
    {
    case CR_STATE_MENU:
    {
        PlayMenuMusic();
        break;
    }
    case CR_STATE_CONNECTED:
    {
        //Remove menu music
        StopMenuMusic();
        break;
    }
    
    default:
        break;
    }
}

bool GameIsPlaying()
{
    return GameGetState() == CR_STATE_CONNECTED || GameGetState() == CR_STATE_VIRUSWIN;
}

void GameInitNetPlayersTable(uint16_t count)
{
    Game* game = GetGame();
    hashtable_init(&game->players, sizeof(NetPlayer), count, 0);
}

uint16_t GameGetPlayerCount()
{
    return hashtable_count(&GetGame()->players);
}

NetPlayer* GameGetAllPlayers()
{
    return hashtable_items(&GetGame()->players);
}

void GameInitNetPlayer(PlayerData* data)
{
    Game* game = GetGame();

    NetPlayer* prev = GameGetNetPlayer(data->id);
    if(prev != NULL)
    {
        LogInfo("PLAYER ID EXISTS, DISPOSING PREVIOUS INSTANCE");
        GameDisposeNetPlayer(prev);
    }
    // assert(GameGetNetPlayer(data->id) == NULL, "PLAYER ID ALREADY EXISTS");

    NetPlayer player;
    InitNetPlayer(&player, data);

    // debug
    LogInfo("ID: %d | x: %d | y: %d | angle: %f | inf: %d", player.data.id, player.data.x, player.data.y, player.data.angle, player.data.infected);
    // debug

    hashtable_insert(&game->players, player.data.id, &player);
}

void GameInitNetPlayers(PlayerData* players, uint16_t count)
{
    Game* game = GetGame();

    GameInitNetPlayersTable(count);
    for (size_t i = 0; i < count; i++)
    {
        GameInitNetPlayer(&players[i]);
    }
}

void GameDisposeNetPlayer(NetPlayer* player)
{
    assert(player != NULL);

    Game* game = GetGame();

    NetPlayerDispose(player);
    hashtable_remove(&game->players, player->data.id);
}

void GameDisposeNetPlayers()
{
    Game* game = GetGame();

    NetPlayer* players = GameGetAllPlayers();
    for (size_t i = 0; i < GameGetPlayerCount(); i++)
    {
        GameDisposeNetPlayer(&players[i]);
    }

    hashtable_term(&game->players);
}

NetPlayer* GameGetNetPlayer(PlayerID id)
{
    Game* game = GetGame();
    NetPlayer* ply = hashtable_find(&game->players, id);
    //assert(ply != NULL, "PLAYER DOESNT EXIST");
    return ply;
}

void ApplyMovementDataToPlayer(PlayerMovementData* data)
{
    NetPlayer* player = GameGetNetPlayer(data->id);
    if (player == NULL) return;

    SDL_LockMutex(player->mutex);
    
    player->data.angle = data->angle;
    player->data.x = data->x;
    player->data.y = data->y;

    SDL_UnlockMutex(player->mutex);
}
