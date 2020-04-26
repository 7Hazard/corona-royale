#pragma once

#include <stdbool.h>
#include <SDL.h>
#include "mouse.h"
#include "events.h"
#include "shared/data.h"

typedef struct Camera
{
    SDL_Rect cameraRect;
} Camera;

typedef struct Player
{
    SDL_Rect rect;
    SDL_mutex* positionMutex;
    SDL_Rect position;
    Camera camera;
    SDL_mutex* infectedMutex;
    bool infected;
    bool mouseClick;
    float angle;
    SDL_Texture* texture;
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
void SetPlayerPosition(Player* player, int x, int y);
void SetPlayerAngle(Player* player, float angle);
void SetPlayerInfected(Player* player, bool infected);
void ApplyPlayerData(Player* player, PlayerData* data);
void GetPlayerPositionData(Player* player, PlayerPositionData* data);
