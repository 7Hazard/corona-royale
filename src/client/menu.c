#include "menu.h"
#include "game.h"
#include "fonts.h"

#include <SDL_FontCache.h>
#include <SDL_image.h> 


void LoadMenu(Menu* menu){
    Game* game = GetGame();

    menu->textureMenu = IMG_LoadTexture(game->renderer,"res/background_menu.jpg");
    menu->textureMenu = IMG_LoadTexture(game->renderer,"res/menu/background_menu.jpg");

    menu->texturePlayButton = IMG_LoadTexture(game->renderer,"res/GreenPlayButton.png");

    menu->textureExitButton = IMG_LoadTexture(game->renderer,"res/ExitButtonR.jpg");
    
    SDL_QueryTexture(menu->textureMenu,NULL,NULL,&menu->width,&menu->heigth);

    menu->rectPlayButton.h = 100;
    menu->rectPlayButton.w = 100;
    menu->rectPlayButton.x = 150;
    menu->rectPlayButton.y = 200;
}



void RenderMenu(){
    Game* game = GetGame();
    Fonts* font = GetFonts();
    SDL_RenderCopy(game->renderer,game->menu.textureMenu,NULL,NULL);
    
    { // Draw CORONA ROYALE text
        static Uint8 r = 0;
        static float theta = 0.f; theta+=0.03f;
        r = ((sin(theta)+1)/2)*255;
        FC_DrawColor(font->comicSansBoldItalic, game->renderer, 200, 50, FC_MakeColor(r, 20, 20, 255), "CORONA\n%s", "ROYALE");
    }
}

void HandleMenuEvents(SDL_Event* event)
{
    Game* game = GetGame();

    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        game->currentState = CR_STATE_RUNNING;
    }
}

