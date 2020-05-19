#include "netplayer.h"

#include "textures.h"
#include "game.h"
#include "shared/log.h"
#include "shared/math.h"

void InitNetPlayer(NetPlayer* np, PlayerData* data)
{
    Textures* tex = GetTextures();

    np->mutex = SDL_CreateMutex();
    np->data = *data; // Copy data
    
    if(np->data.infected)
        np->texture = tex->infectedPlayer;
    else np->texture = tex->healthyPlayer;
}
void NetPlayerDrawCircle(NetPlayer* np)
{
    Game* game = GetGame();
    Textures* tex = GetTextures();
    const int32_t diameter = (np->data.infectionRadius * 2);

    int32_t x = (np->data.infectionRadius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);
    int centreX = ((int)np->data.x) - game->player.camera.cameraRect.x + tex->playerWidth/2;
    int centreY = ((int)np->data.y) - game->player.camera.cameraRect.y + tex->playerHeight/2;
    SDL_SetRenderDrawColor(game->renderer, 255,0,0,255);

    while (x >= y) 
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(game->renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(game->renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(game->renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(game->renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(game->renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(game->renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(game->renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(game->renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
void RenderNetPlayer(NetPlayer* np)
{
    Game* game = GetGame();
    Textures* tex = GetTextures();
    Player* player = &game->player;
    Camera* camera = &player->camera;

    SDL_Rect rect;
    rect.w = tex->playerWidth; rect.h = tex->playerHeight;

    SDL_LockMutex(np->mutex);
    int npx = np->data.x - rect.w;
    int npy = np->data.y - rect.h;
    SDL_UnlockMutex(np->mutex);

    // Render relative to the camera
    int camx = camera->cameraRect.x, camy = camera->cameraRect.y, camw = camera->cameraRect.w, camh = camera->cameraRect.h;
    npx =npx - (camx - rect.w);
    npy =npy - (camy - rect.h);
    rect.x = npx;
    rect.y = npy;
    
    // render player
    SDL_RenderCopyEx(game->renderer, np->texture, NULL, &rect, np->data.angle, NULL, SDL_FLIP_NONE);
    
    if (np->data.infected)
    {
        NetPlayerDrawCircle(np);
    }
    
    if(game->player.infected && !np->data.infected && !GameIsWithinCamera(npx, npy))
    {
        npx += rect.w/2;
        npy += rect.h/2;
        
        // center pos relative to cam
        int cx = camw/2, cy = camh/2;
        float ix = 0, iy = 0;
        SDL_Rect arrow;
        arrow.w = 15;
        arrow.h = 30;
        
        // upper cam line
        if(LineLineIntersection(cx, cy, npx, npy, 0, 0, camw, 0, &ix, &iy))
        {
            LogInfo("UPPER %f %f", ix, iy);
            ix -= arrow.w/2;
        }
        // right cam line
        else if(LineLineIntersection(cx, cy, npx, npy, camw, 0, camw, camh, &ix, &iy))
        {
            LogInfo("RIGHT %f %f", ix, iy);
            ix -= arrow.w;
            iy -= arrow.h/2;
        }
        // lower cam line
        else if(LineLineIntersection(cx, cy, npx, npy, 0, camh, camw, camh, &ix, &iy))
        {
            LogInfo("LOWER %f %f", ix, iy);
            ix -= arrow.w/2;
            iy -= arrow.h;
        }
        // left cam line
        else if(LineLineIntersection(cx, cy, npx, npy, 0, 0, 0, camh, &ix, &iy))
        {
            LogInfo("LEFT %f %f", ix, iy);
            iy -= arrow.h/2;
        }
        else {
            // LogInfo("WHAT??");
            return;
        }
        
        arrow.x = ix;
        arrow.y = iy;

        float angle = GetAngle(cx, cy, npx, npy)-90;
        SDL_RenderCopyEx(game->renderer, tex->arrowIndicator, NULL, &arrow, angle, NULL, SDL_FLIP_NONE);

        if (np->data.infected)
        {
            DrawCircle(player);
        }
    }
}

void NetPlayerDispose(NetPlayer* np)
{
    SDL_DestroyMutex(np->mutex);
}

void ApplyDataToNetPlayer(NetPlayer* np, PlayerData* data)
{
    SDL_LockMutex(np->mutex);

    np->data = *data;

    SDL_UnlockMutex(np->mutex);
}
