#pragma once

#include <SDL_mutex.h>
#include "shared/data.h"

typedef struct NetPlayer
{
    SDL_mutex* mutex;
    PlayerData data;
    SDL_Texture* texture;
} NetPlayer;

void InitNetPlayer(NetPlayer* np, PlayerData* data);
void RenderNetPlayer(NetPlayer* np);
void NetPlayerDispose(NetPlayer* np);
