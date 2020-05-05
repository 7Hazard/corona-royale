#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>
#include <time.h> 

#define HASHTABLE_IMPLEMENTATION
#include "shared/hashtable.h"
#undef HASHTABLE_IMPLEMENTATION

#include "memory.h"
#include "collision.h"
#include "events.h"
#include "player.h"
#include "game.h"
#include "audio.h"
#include "timer.h"
#include "netplayer.h"
#include "gamenet.h"

int main(int argc, const char *argv[])
{
    SDL_Log("Corona Royale\n");

    Game* game = GetGame();
    
    GameNetStartThread();

    FC_Font* Sans = FC_CreateFont();  
    FC_LoadFont(Sans, game->renderer, "res/fonts/ComicSansMS3.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_BOLD|TTF_STYLE_ITALIC);
    while (game->running)
    {
        Uint32 frameStart = GameStartFrame();
        //// FRAME START
        
        { /////////// STATE UPDATES PHASE BEGIN ///////////
            HandleEvents();
            OnPlayerUpdate(&game->player);
        } /////////// STATE UPDATES PHASE END /////////////
        
        { /////////// RENDERING PHASE BEGIN /////////
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderClear(game->renderer);

            // Draw background
            SDL_RenderCopy(game->renderer, game->background, &game->player.camera.cameraRect, NULL);

            // Render all net players
            NetPlayer* players = GetAllPlayers();
            for (size_t i = 0; i < GetPlayerCount(); i++)
            {
                RenderNetPlayer(&players[i]);
            }

            OnPlayerRender(&game->player);
            { // Draw CORONA ROYALE text
                static Uint8 r = 0;
                static float theta = 0.f; theta+=0.03f;
                r = ((sin(theta)+1)/2)*255;
                FC_DrawColor(Sans, game->renderer, 200, 50, FC_MakeColor(r, 20, 20, 255), "CORONA\n%s", "ROYALE");
            }
            RendererTimer(&game->timer);

            SDL_RenderPresent(game->renderer);
        } /////////// RENDERING PHASE END ///////////
        
        //// FRAME END
        GameEndFrame(frameStart);
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    StopAudio();
    return 0;
}

