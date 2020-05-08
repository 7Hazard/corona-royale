
#include "menu.h"
#include "game.h"
#include "fonts.h"
#include <string.h>
#include <stdio.h>

#include <SDL_FontCache.h>
#include <SDL_image.h> 

void LoadMenu(Menu* menu){
    Game* game = GetGame();

    game->menu.textBoxClick = false;

    menu->textureMenu = IMG_LoadTexture(game->renderer,"res/menu/background_menu.jpg");
    menu->textureLogo = IMG_LoadTexture(game->renderer,"res/menu/CoronaRoyalBackground.png");
    menu->textureTextBox = IMG_LoadTexture(game->renderer,"res/TextBoxCoronaRoyalr2.png");

    menu->textLength = -1;

    menu->logoRect.x = 50;
    menu->logoRect.y = 30;
    menu->logoRect.w = 320;
    menu->logoRect.h = 250;

    menu->serverTextRect.x = 50;
    menu->serverTextRect.y = 30;
    menu->serverTextRect.w = 50;
    menu->serverTextRect.h = 20;
    
    menu->textBoxRect.x = 100;
    menu->textBoxRect.y = 340;
    menu->textBoxRect.h = 80;
    menu->textBoxRect.w = 250;

    menu->textColor.r = 255;
    menu->textColor.g = 255;
    menu->textColor.b = 255;
    menu->textColor.a = 255;

    menu->boxOutlineColor.r = 0;
    menu->boxOutlineColor.g = 0;
    menu->boxOutlineColor.b = 0;
    menu->boxOutlineColor.a = 0;

    menu->boxBackgroundcolor.r = 0;
    menu->boxBackgroundcolor.g = 0;
    menu->boxBackgroundcolor.b = 0;
    menu->boxBackgroundcolor.a = 0;
}

void RenderMenu(){
    Game* game = GetGame();;
    Fonts* font = GetFonts();

    SDL_RenderCopy(game->renderer,game->menu.textureMenu,NULL,NULL);
    SDL_RenderCopy(game->renderer,game->menu.textureLogo,NULL,&game->menu.logoRect);
    SDL_RenderCopy(game->renderer,game->menu.textureTextBox,NULL,&game->menu.textBoxRect);

    FC_DrawColor(font->openSans, game->renderer, 110, 365, FC_MakeColor(255, 255, 255, 255), game->menu.textInTextBox);
    FC_DrawColor(font->openSans, game->renderer, 10, 365 , FC_MakeColor(255, 255, 255, 255), "Enter IP:");
}

void HandleMenuEvents(SDL_Event* event)
{
    Game* game = GetGame();

    if (event->type == SDL_KEYDOWN && game->menu.textBoxClick == true)
    {
        SDL_StartTextInput();

        SDL_Event inputEvent;
        while(SDL_PollEvent(&inputEvent))
        {
            if(inputEvent.type == SDL_TEXTINPUT)
            {
                /* Add new text onto the end of our text */
                game->menu.textLength++;
                game->menu.textInTextBox[game->menu.textLength]+= *inputEvent.text.text;
            }
        }
    }
    else if(event->key.keysym.sym == SDLK_BACKSPACE && game->menu.textLength >= 0)
    {
        game->menu.textInTextBox[game->menu.textLength] = '\0';
        game->menu.textLength--;
    }
    else if(event->key.keysym.sym == SDLK_RETURN)
    {
        GameNetConnect();
    }
    else if(event->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&game->menu.mouseX,&game->menu.mouseY);
                //If the mouse is over the textbox
        if( (game->menu.mouseX > game->menu.textBoxRect.x ) && ( game->menu.mouseX < game->menu.textBoxRect.x + game->menu.textBoxRect.w ) && ( game->menu.mouseY > game->menu.textBoxRect.y  ) && ( game->menu.mouseY < game->menu.textBoxRect.y  + game->menu.textBoxRect.h ) )
        {
            game->menu.textBoxClick = true;
        }
    }
}
