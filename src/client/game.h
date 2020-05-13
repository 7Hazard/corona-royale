#pragma once

#include <stdbool.h>
#include <SDL.h>

#include "player.h"
#include "timer.h"
#include "netplayer.h"
#include "menu.h"

#include "shared/hashtable.h"

#define CR_WINDOW_W 375
#define CR_WINDOW_H 667

typedef enum GameState {
    CR_STATE_PRELOAD,
    CR_STATE_MENU,
    CR_STATE_LOADGAME,
    CR_STATE_CONNECTED,
    CR_STATE_VIRUSWIN
} GameState; 

typedef struct Game
{
    bool running;

    SDL_mutex* stateMutex;
    GameState state;

    SDL_Window* window;
    SDL_Renderer* renderer;

    Uint32 fps;

    SDL_Texture *background;
    int mapWidth, mapHeight;
    Player player;
    Timer timer;
    
    hashtable_t players;
} Game;

Game* GetGame();
void DisposeGame();
Uint32 GameStartFrame();
void GameEndFrame(Uint32 framestart);
GameState GameGetState();
void GameSetState(GameState state);
bool GameIsPlaying();
void GameInitNetPlayersTable(uint16_t count);
void GameInitNetPlayer(PlayerData* data);
void GameInitNetPlayers(PlayerData* players, uint16_t count); // not used
void GameDisposeNetPlayer(NetPlayer* player);
void GameDisposeNetPlayers();
NetPlayer* GameGetNetPlayer(PlayerID id);
uint16_t GameGetPlayerCount();
NetPlayer* GameGetAllPlayers();
void ApplyMovementDataToPlayer(PlayerMovementData* data);
bool GameIsWithinCamera(int x, int y);
