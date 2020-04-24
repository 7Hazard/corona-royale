#pragma once

#include <stdbool.h>
#include <SDL.h>
#include "mouse.h"
#include "events.h"
#include "shared/data.h"

typedef struct Camera
{
    SDL_Rect cameraRect;
    SDL_Rect drawingRect;
} Camera;

typedef struct Player
{
    SDL_Rect rect;
    SDL_mutex* positionMutex;
    SDL_Rect position;
    Camera camera;
    SDL_mutex* infectedMutex;
    bool infected;
    bool left;
    bool right;
    bool up;
    bool down;
    Mouse mouse;
    SDL_Texture* image;
    int frameWidth, frameHeight;
    int textureWidth, textureHeight;
} Player;

void CreatePlayer(Player* player, int xPos, int yPos);
void HandlePlayerEvents(SDL_Event *event);
void OnPlayerUpdate(Player* player);
void OnPlayerRender(Player* player);
bool IsPlayerMoving(Player* player);
void SetPlayerPosition(Player* player, int x, int y);
void SetPlayerAngle(Player* player, float angle);
void SetPlayerInfected(Player* player, bool infected);
void ApplyPlayerData(Player* player, PlayerData* data);
void GetPlayerPositionData(Player* player, PositionData* data);
