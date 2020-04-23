

#include "menu.h"
#include "game.h"


void LoadMenu(Menu* menu){
    Game* game = GetGame();

    SDL_Surface* surfaceMenuBackground = IMG_Load("res/background_menu.jpg");
    menu->textureMenu = SDL_CreateTextureFromSurface(game->renderer, surfaceMenuBackground);

    SDL_Surface* surfacePlayButton = IMG_Load("res/GreenPlayButton.png");
    menu->texturePlayButton = SDL_CreateTextureFromSurface(game->renderer,surfacePlayButton);

    SDL_Surface* surfaceExitButton = IMG_Load("res/ExitButtonR.jpg");
    menu->texturePlayButton = SDL_CreateTextureFromSurface(game->renderer,surfacePlayButton);
}