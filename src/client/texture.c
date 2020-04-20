#include "texture.h"
#include "game.h"
#include <SDL_image.h>

SDL_Texture* LoadTexture(const char* filename)
{
    Game *game = GetGame();
    SDL_Surface *surf = IMG_Load(filename);
    SDL_Texture *bTexture = SDL_CreateTextureFromSurface(game->renderer, surf);
    SDL_FreeSurface(surf);
    if (bTexture == NULL)
    {
        char msg[256];
        sprintf(msg, "Could not load texture %s\nError: %s",filename, IMG_GetError());
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "Could not load texture",
            msg,
            NULL);
        abort();
    }

    return bTexture;
}