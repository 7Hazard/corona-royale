#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

typedef struct Audio
{
  Mix_Chunk* cough;
  Mix_Chunk* steps;
} Audio;

Audio* GetAudio();
void StopAudio();
