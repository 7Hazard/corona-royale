#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

typedef struct Audio
{
  Mix_Chunk* cough;
  Mix_Chunk* steps;
  Mix_Music* menuMusic;
  Mix_Chunk* virusWin;
} Audio;

Audio* GetAudio();
void DisposeAudio();
void PlayMenuMusic();
void StopMenuMusic();
void PlayVirusWin();
