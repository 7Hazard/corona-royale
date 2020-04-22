#pragma once

#include <stdbool.h>
#include <SDL.h>
#include "player.h"
#define PI 3.14159265358979323846

typedef struct Mouse
{
    int x;
    int y;
} Mouse;

float GetAngle(int x1, int x2, int y1, int y2);
Mouse GetMouse();