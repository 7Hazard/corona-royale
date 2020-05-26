#include <stdio.h>
#include <stdbool.h>
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
#include "fonts.h"
#include "timer.h"
#include "netplayer.h"
#include "gamenet.h"

#include "shared/log.h"

void NetworkDisconnected(TCPsocket socket)
{
    LogInfo("DISCONNECTED");

    GameSetState(CR_STATE_MENU);

    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_INFORMATION,
        "Connection",
        "Disconnected from server",
        NULL
    );
}

int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    Game* game = GetGame();
    Fonts* fonts = GetFonts();

    GameSetState(CR_STATE_MENU);

    while (game->running)
    {
        Uint32 frameStart = GameStartFrame();
        //// FRAME START
        
        { /////////// STATE UPDATES PHASE BEGIN ///////////
            HandleEvents();
            if(GameIsPlaying())
            {
                OnPlayerUpdate(&game->player);
            }
        } /////////// STATE UPDATES PHASE END /////////////

        { /////////// RENDERING PHASE BEGIN /////////
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderClear(game->renderer);

            
            if(GameGetState() == CR_STATE_MENU)
            {
                RenderMenu();
            }
            else if(GameIsPlaying())
            {
                // Draw background
                SDL_RenderCopy(game->renderer, game->background, &game->player.camera.cameraRect, NULL);

                // Render all net players
                if(GameGetState())
                {
                    NetPlayer* players = GameGetAllPlayers();
                    for (size_t i = 0; i < GameGetPlayerCount(); i++)
                    {
                        RenderNetPlayer(&players[i]);
                    }
                }

                OnPlayerRender(&game->player);
                // { // Draw CORONA ROYALE text
                //     static Uint8 r = 0;
                //     static float theta = 0.f; theta+=0.03f;
                //     r = ((sin(theta)+1)/2)*255;
                //     FC_DrawColor(fonts->openSans, game->renderer, 200, 50, FC_MakeColor(r, 20, 20, 255), "CORONA\n%s", "ROYALE");
                // }
                // RendererTimer(&game->timer);

                if(GameGetState() == CR_STATE_VIRUSWIN)
                {
                    // background
                    SDL_Rect rect; rect.x = 20; rect.y = 90; rect.w = 335; rect.h = 80;
                    SDL_SetRenderDrawColor(game->renderer, 70, 70, 70, 150); // alpha doesnt work
                    SDL_RenderFillRect(game->renderer, &rect);

                    // text
                    SDL_Color color = {143, 21, 21, 230};
                    FC_DrawColor(fonts->openSansBold, game->renderer, 30, 100, color, "ALLA AROPPA KREN VAIRUS");
                    FC_DrawColor(fonts->openSansBold, game->renderer, 60, 130, color, "%d people got infected!", GameGetPlayerCount());
                }
            }

            SDL_RenderPresent(game->renderer);
        } /////////// RENDERING PHASE END ///////////
        
        //// FRAME END
        GameEndFrame(frameStart);
    }

    DisposeGame();
    DisposeTextures();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    DisposeAudio();
    return 0;
}

