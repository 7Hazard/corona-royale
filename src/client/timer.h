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
    // TTF_Font* Sans;
    SDL_Color White;
    SDL_Color Red;
    SDL_Surface* surfaceTime;
    SDL_Texture* textureTime;
    SDL_Rect rect;
    SDL_Rect position;
    char timerBuffer[50];
}Timer;


void CreateTimer(Timer* timer);
void ResetSeconds(Timer* timer);
void UpdateTimer(Timer* timer);
void RendererTimer(Timer* timer);
