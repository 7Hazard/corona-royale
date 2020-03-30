#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

void InitAudio();
void StopAudio();

extern Mix_Chunk* cough;
extern Mix_Chunk* steps;
