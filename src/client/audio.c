#include <stdio.h>
#include <stdbool.h>
#include <SDL_mixer.h>

#include "audio.h"
#include "game.h"
#include "shared/log.h"

Audio* GetAudio()
{
    // Will initialize after first call
    static bool inited = false;
    static Audio audio;
    if(!inited)
    {
        inited = true;

        if(Mix_Init(0)!=0) 
        {
            SDL_Log("Mix_Init: %s\n", SDL_GetError());
            abort();
            exit(2);
        }
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
            
        // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
        // Mix_Music *backgroundSound = Mix_LoadMUS("res/Heroic Demise (New).mp3");
        audio.cough = Mix_LoadWAV("res/cough.wav");
        audio.steps = Mix_LoadWAV("res/FootstepsGrass.wav");
        audio.menuMusic = Mix_LoadMUS("res/Resilience.mp3");
        // Mix_PlayMusic(backgroundSound, -1);
        // DONT FORGET TO INITIALIZE ALL MEMBERS OF THE STRUCT
    }

    return &audio;
}


void DisposeAudio()
{
    Audio* audio = GetAudio();

    // DONT FORGET TO FREE ALL SOUNDS
    // Mix_FreeChunk(walking);
    // Mix_FreeMusic(backgroundSound);
    Mix_FreeChunk(audio->cough);
    Mix_FreeChunk(audio->steps);
    Mix_FreeMusic(audio->menuMusic);
    // DONT FORGET TO FREE ALL SOUNDS

    Mix_CloseAudio();
    Mix_Quit();
}

void PlayMenuMusic()
{
    Audio* audio = GetAudio();
    if(GameGetState() == CR_STATE_MENU)
    {
        if(Mix_PlayMusic(audio->menuMusic, -1) == -1)
        {
            LogInfo("Mix_PlayMusic: %s\n", Mix_GetError());
        }
        else if(!Mix_PlayingMusic() == 0)
        {
            Mix_VolumeMusic(20);
            Mix_PlayMusic(audio->menuMusic, -1);
        }
    }
}

void StopMenuMusic()
{
    Audio* audio = GetAudio();
    
    Mix_HaltMusic(audio->menuMusic);
}
