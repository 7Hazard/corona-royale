#include <stdio.h>
#include <SDL.h>
#include "main.h"

void HandleBorders()
{
    if (rect.x < 0 && rect.y < 0)
    {
        rect.x = 0;
        rect.y = 0;
    }
    else if (rect.x > 420 && rect.y < 0)
    {
        rect.x = 420;
        rect.y = 0;
    }
    else if (rect.x < 0 && rect.y > 720)
    {
        rect.x = 0;
        rect.y = 720;
    }
    else if (rect.x > 420 && rect.y > 720)
    {
        rect.x = 420;
        rect.y = 720;
    }
    else if (rect.x < 0)
    {
        rect.x = 0;
    }
    else if (rect.x > 420)
    {
        rect.x = 420;
    }
    else if (rect.y < 0)
    {
        rect.y = 0;
    }
    else if (rect.y > 720)
    {
        rect.y = 720;
    }
}




