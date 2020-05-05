#include "netplayer.h"

#include "textures.h"
#include "game.h"

void InitNetPlayer(NetPlayer* np, PlayerData* data)
{
    Textures* tex = GetTextures();

    np->mutex = SDL_CreateMutex();
    np->data = *data; // Copy data
    np->texture = tex->healthyPlayer;
}

void RenderNetPlayer(NetPlayer* np)
{
    Game* game = GetGame();
    Textures* tex = GetTextures();

    SDL_Rect rect;
    rect.w = tex->playerWidth; rect.h = tex->playerHeight;

    SDL_LockMutex(np->mutex);

    // Render relative to the camera
    rect.x = np->data.x - game->player.camera.cameraRect.x;
    rect.y = np->data.y - game->player.camera.cameraRect.y;
    SDL_RenderCopyEx(game->renderer, np->texture, NULL, &rect, np->data.angle, NULL, SDL_FLIP_NONE);
    
    SDL_UnlockMutex(np->mutex);
}

void DisposeNetPlayer(NetPlayer* np)
{
    SDL_DestroyMutex(np->mutex);
    // free(np);
}

void ApplyDataToNetPlayer(NetPlayer* np, PlayerData* data)
{
    SDL_LockMutex(np->mutex);

    np->data = *data;

    SDL_UnlockMutex(np->mutex);
}
