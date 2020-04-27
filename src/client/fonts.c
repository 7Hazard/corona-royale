#include <stdio.h>
#include <stdlib.h>
#include <SDL_FontCache.h>

#include "game.h"
#include "fonts.h"

Fonts* GetFonts()
{
    // Will initialize after first call
    static bool inited = false;
    static Fonts fonts;
    if(inited == false)
    {
        inited = true;

        Game* game = GetGame();

        fonts.comicSansBoldItalic = FC_CreateFont();
        FC_LoadFont(fonts.comicSansBoldItalic, game->renderer, "res/fonts/ComicSansMS3.ttf", 20, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_BOLD|TTF_STYLE_ITALIC);

        fonts.openSansBold = FC_CreateFont();
        FC_LoadFont(fonts.openSansBold, game->renderer, "res/fonts/OpenSans-Semibold.ttf", 20, FC_MakeColor(255,255,255,255), TTF_STYLE_BOLD);
    }

    return &fonts;
}
