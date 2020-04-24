#include "menu.h"
#include "game.h"

#include <SDL_image.h> 


void LoadMenu(Menu* menu){
    Game* game = GetGame();

    menu->textureMenu = IMG_LoadTexture(game->renderer,"C:/Users/Dell/Desktop/hi1038-project/res/menu/background_menu.jpg");

    menu->texturePlayButton = IMG_LoadTexture(game->renderer,"C:/Users/Dell/Desktop/hi1038-project/res/GreenPlayButton.png");

    menu->textureExitButton = IMG_LoadTexture(game->renderer,"res/ExitButtonR.jpg");
    
    SDL_QueryTexture(menu->textureMenu,NULL,NULL,&menu->width,&menu->heigth);

    menu->rectPlayButton.h = 100;
    menu->rectPlayButton.w = 100;
    menu->rectPlayButton.x = 150;
    menu->rectPlayButton.y = 200;
}



void RenderMenu(){

    Game* game = GetGame();
    SDL_RenderCopy(game->renderer,game->menu.textureMenu,NULL,NULL);

}

void HandleMenuEvents(SDL_Event* event){

    Game* game = GetGame();

     if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        game->currentState = CR_STATE_RUNNING;
    }
}

