#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>


typedef struct{
    bool mStarted;
    bool mLast10Sek;
    int mCurrentTime;
    int mMinuteTime;
    int mPrevTime;
    int mDeltaTime;
    float mDeltaConvert;
    
    SDL_Texture* textureGrayBox;
    SDL_Rect grayBoxRect;
    SDL_Texture* textureTimerClock;
    SDL_Rect timerClockRect;
    SDL_Color White;
    SDL_Color Red;
    SDL_Rect rect;
    char timerBuffer[50];
}Timer;


void CreateTimer(Timer* timer);
void ResetSeconds(Timer* timer);
void UpdateTimer(Timer* timer);
void RendererTimer(Timer* timer);
