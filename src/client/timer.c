#include "timer.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"

void CreateTimer(Timer *timer)
{
    TTF_Init(); // va tvungen att lägga den här för att initiera Sans

    timer->mCurrentTime = 0;
    timer->mDeltaConvert = 0;
    timer->mDeltaTime = 0;
    timer->mLast10Sek = false;
    timer->mMinuteTime = 0;
    timer->mPrevTime = 0;
    timer->mStarted = true;

    timer->Sans = TTF_OpenFont("res/fonts/OpenSans-Semibold.ttf", 24);
    if (!timer->Sans)
    {
        abort();
    }

    timer->White.a = 255;
    timer->White.b = 255;
    timer->White.g = 255;
    timer->White.r = 255;

    timer->Red.a = 0;
    timer->Red.b = 0;
    timer->Red.g = 0;
    timer->Red.r = 255;

    timer->rect.x = 0;
    timer->rect.y = 0;
    timer->rect.w = 100;
    timer->rect.h = 100;

    timer->position.x = 10;
    timer->position.y = 10;
    timer->position.w = 50;
    timer->position.h = 50;

    timer->surfaceTime = NULL;
   
}

void ResetSeconds(Timer *timer)
{
    timer->mPrevTime = timer->mCurrentTime;
    timer->mDeltaTime = (timer->mCurrentTime - timer->mPrevTime) / 1000;
}

void UpdateTimer(Timer *timer)
{
    timer->mCurrentTime = SDL_GetTicks();
    if (timer->mStarted == true)
    {
        timer->mDeltaTime = (timer->mCurrentTime - timer->mPrevTime) / 1000;
        if (timer->mDeltaTime == 59)
        { // add minute
            timer->mMinuteTime += 1;
            ResetSeconds(timer);
        }
        if (timer->mMinuteTime == 1 && timer->mDeltaTime == 30)
        {
            timer->mLast10Sek = true;
        }
        if (timer->mMinuteTime == 2)
        {
            timer->mStarted = false;
            ResetSeconds;
        }
    }
}

void RendererTimer(Timer *timer)
{
    Game* game = GetGame();

    UpdateTimer(&game->timer);
    sprintf(game->timer.timerBuffer, "%d:%d", game->timer.mMinuteTime, game->timer.mDeltaTime); // copy string (timer.mStartTicks)to buffer

    if (!game->timer.mLast10Sek)
    {
        game->timer.surfaceTime = TTF_RenderText_Solid(game->timer.Sans, game->timer.timerBuffer, game->timer.White);
    }
    else if (game->timer.mLast10Sek)
    {
        game->timer.surfaceTime = TTF_RenderText_Solid(game->timer.Sans, game->timer.timerBuffer, game->timer.Red);
    }

    game->timer.textureTime = SDL_CreateTextureFromSurface(game->renderer, game->timer.surfaceTime); //now you can convert it into a texture
    SDL_RenderCopy(game->renderer, game->timer.textureTime, NULL, &game->timer.rect);
}