#pragma once

#include <stdbool.h>
#include <SDL.h>
#include "events.h"
#include "shared/data.h"

typedef struct Camera
{
    SDL_Rect cameraRect;
} Camera;

typedef struct Player
{
    SDL_Rect rect;
    SDL_mutex* movementMutex;
    Camera camera;
    SDL_mutex* infectedMutex;
    bool infected;
    bool mouseClick;
    bool moving;
    float angle;
    float x,y;
    SDL_Texture* texture;
    PlayerID id;
    int radius;
    int speed;
} Player;

void CreatePlayer(Player* player, float xPos, float yPos);
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
void GetPlayerMovementData(Player* player, PlayerMovementData* data);
void DrawCircle(Player* player);
