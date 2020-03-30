#include <stdio.h>
#include <SDL_mixer.h>

Mix_Chunk* cough = NULL;
Mix_Chunk* steps = NULL; 

void InitAudio()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    	
    // Mix_Music *backgroundSound = Mix_LoadMUS("res/Heroic Demise (New).mp3");
    cough = Mix_LoadWAV("res/cough.wav");
    steps = Mix_LoadWAV("res/footstep.wav");
    // Mix_PlayMusic(backgroundSound, -1);
}
void StopAudio()
{
    // Mix_FreeChunk(walking);
    // Mix_FreeMusic(backgroundSound);
    Mix_FreeChunk(cough);
    Mix_FreeChunk(steps);
    Mix_CloseAudio();
    Mix_Quit();
}
