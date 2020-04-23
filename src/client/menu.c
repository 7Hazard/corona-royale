#include "menu.h"
#include "game.h"


void LoadMenu(Menu* menu){
    Game* game = GetGame();

    menu->textureMenu = IMG_LoadTexture(game->renderer,"res/background_menu.jpg");

    menu->texturePlayButton = IMG_LoadTexture(game->renderer,"res/GreenPlayButton.png");

    menu->textureExitButton = IMG_LoadTexture(game->renderer,"res/ExitButtonR.jpg");
    
    SDL_QueryTexture(menu->textureMenu,NULL,NULL,&menu->width,&menu->heigth);

}

void ShowMenu(Menu* menu){
    LoadMenu(&menu);
    Game* game = GetGame();
}

void RenderMenu(){

    Game* game = GetGame();

    SDL_RenderCopy(game->renderer,game->menu.textureMenu,NULL,NULL);
}