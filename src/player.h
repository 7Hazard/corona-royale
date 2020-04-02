#pragma once

#include <stdbool.h>
#include <SDL.h>

typedef struct
{
    SDL_Rect rect;
    SDL_Rect playerPosition;
    SDL_Rect cameraRect;
    bool infected;
    bool left;
    bool right;
    bool up;
    bool down;
    bool noMovement;

} Player;

void CreatePlayer(Player* player, int xPos, int yPos,int frameWidth, int frameHeight);
