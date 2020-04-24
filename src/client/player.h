#pragma once

#include <stdbool.h>
#include <SDL.h>
#include "mouse.h"
#include "events.h"




typedef struct Camera
{
    SDL_Rect cameraRect;
}Camera;


typedef struct Player
{
    SDL_Rect rect;
    SDL_Rect position;
    Camera camera;
    bool infected;
    bool left;
    bool right;
    bool up;
    bool down;
    bool mouseClick;
    SDL_Texture* image;
    float angle;
    int frameWidth, frameHeight;
    int textureWidth, textureHeight;
} Player;

void CreatePlayer(Player* player, int xPos, int yPos);
void HandlePlayerEvents(SDL_Event *event);
void OnPlayerUpdate(Player* player);
void RotatePlayer();
void OnPlayerRender(Player* player);
void MoveTowardsMouse(Player *player);
bool IsPlayerMoving(Player* player);
