#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

typedef struct Audio
{
  Mix_Chunk* cough;
  Mix_Chunk* steps;
} Audio;

EXTERN Audio* GetAudio();
EXTERN void StopAudio();
