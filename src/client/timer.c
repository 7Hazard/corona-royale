#include "timer.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"


void CreateTimer(Timer *timer)
{
    timer->mCurrentTime = 0;
    timer->mDeltaConvert = 0;
    timer->mDeltaTime = 0;
    timer->mLast10Sek = false;
    timer->mMinuteTime = 0;
    timer->mPrevTime = 0;
    timer->mStarted = true;

    Game* game = GetGame();
    
    timer->Sans = FC_CreateFont();
    FC_LoadFont(timer->Sans,game->renderer,"res/fonts/OpenSans-Semibold.ttf", 
    24,FC_MakeColor(255, 255, 255, 255),TTF_STYLE_NORMAL);
    if (!timer->Sans)
    {
        abort();
    }
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
    UpdateTimer(timer);
    sprintf(timer->timerBuffer, "%d:%d", timer->mMinuteTime, timer->mDeltaTime); // copy string (timer.mStartTicks)to buffer

    if (!timer->mLast10Sek)
    {
        FC_DrawColor(timer->Sans, game->renderer, 0, 50, FC_MakeColor(255, 255, 255, 255), timer->timerBuffer);
    }
    else if (timer->mLast10Sek)
    {
         timer->surfaceTime = TTF_RenderText_Solid(timer->Sans, timer->timerBuffer, timer->Red);
        FC_DrawColor(timer->Sans, game->renderer, 0, 50, FC_MakeColor(255, 0, 0, 255),timer->timerBuffer);
    }
}