#pragma once

#include <math.h>
#include "game.h"
#include <SDL_image.h>
#include <stdio.h>
#include "mouseInput.h"


float getAngle(int x1, int x2, int y1, int y2)
{
    float angle = -90 + atan2(y1 - y2, x1 - x2) * (180 / PI);
    return angle >= 0 ? angle : 360 +angle;
}
Mouse* getMouse()
{
    Mouse *mouse;
    SDL_GetMouseState(mouse->x, mouse->y);
    return &mouse;
}