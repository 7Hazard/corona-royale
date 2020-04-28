#include "textures.h"
#include "game.h"
#include <SDL_image.h>

SDL_Texture* LoadTexture(const char* filename);

Textures* GetTextures()
{
    static bool inited = false;
    static Textures tex;
    if(inited == false)
    {
        // LOAD ALL TEXTURES THAT ARE GOING TO BE USED
        tex.grass = LoadTexture("res/grass.png");

        tex.healthyPlayer = LoadTexture("res/circleblue.png");
        tex.infectedPlayer = LoadTexture("res/circlered.png");
        SDL_QueryTexture(tex.healthyPlayer, NULL, NULL, &tex.playerWidth, &tex.playerHeight);
    }

    return &tex;
}

void DisposeTextures()
{
    Textures* tex = GetTextures();

    // DESTROY ALL LOADED TEXTURES
    SDL_DestroyTexture(tex->grass);
    SDL_DestroyTexture(tex->healthyPlayer);
    SDL_DestroyTexture(tex->infectedPlayer);
}

SDL_Texture* LoadTexture(const char* filename)
{
    Game *game = GetGame();
    SDL_Texture *bTexture = IMG_LoadTexture(game->renderer, filename);
    if (bTexture == NULL)
    {
        char msg[256];
        sprintf(msg, "Could not load texture %s\nError: %s", filename, IMG_GetError());
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Could not load texture",
            msg,
            NULL);
        abort();
    }

    return bTexture;
}