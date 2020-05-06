#pragma once

#include <SDL_FontCache.h>

typedef struct Fonts
{
    FC_Font* comicSansBoldItalic;
    FC_Font* openSansBold;
    FC_Font* openSans;
} Fonts;

Fonts* GetFonts();
