#include "menu.h"
#include "game.h"
#include "fonts.h"

#include <SDL_FontCache.h>
#include <SDL_image.h> 


void LoadMenu(Menu* menu){
    Game* game = GetGame();

    menu->textureMenu = IMG_LoadTexture(game->renderer,"res/menu/background_menu.jpg");
    menu->textureLogo = IMG_LoadTexture(game->renderer,"res/menu/CoronaRoyalBackground.png");

    menu->logoRect.x = 50;
    menu->logoRect.y = 30;
    menu->logoRect.w = 300;
    menu->logoRect.h = 250;
    
}



void RenderMenu(){
    Game* game = GetGame();
    Fonts* font = GetFonts();
    SDL_RenderCopy(game->renderer,game->menu.textureMenu,NULL,NULL);
    SDL_RenderCopy(game->renderer,game->menu.textureLogo,NULL,&game->menu.logoRect);

}

void HandleMenuEvents(SDL_Event* event)
{
    Game* game = GetGame();

    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        game->currentState = CR_STATE_RUNNING;
    }
}

