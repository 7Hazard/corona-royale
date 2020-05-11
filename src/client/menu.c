#include <string.h>
#include <stdio.h>
#include <SDL_FontCache.h>


void LoadMenu(Menu *menu)
{
    Game *game = GetGame();

    game->menu.textBoxClick = false;
    game->menu.cursorBlink = false;
    game->menu.delay = 0;

    menu->textureMenu = IMG_LoadTexture(game->renderer, "res/menu/background_menu.jpg");
    menu->textureLogo = IMG_LoadTexture(game->renderer, "res/menu/CoronaRoyalBackground.png");
    menu->textureTextBox = IMG_LoadTexture(game->renderer, "res/TextBoxCoronaRoyalr2.png");

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

    menu->textCursorRect.x = 110;
    menu->textCursorRect.y = 366;
    menu->textCursorRect.w = 1;
    menu->textCursorRect.h = 25;

    menu->textCursorColor.r = 255;
    menu->textCursorColor.g = 255;
    menu->textCursorColor.b = 255;
    menu->textCursorColor.a = 255;
}

void RenderMenu()
{
    Game *game = GetGame();
    Fonts *font = GetFonts();

    SDL_RenderCopy(game->renderer, game->menu.textureMenu, NULL, NULL);
    SDL_RenderCopy(game->renderer, game->menu.textureLogo, NULL, &game->menu.logoRect);
    SDL_RenderCopy(game->renderer, game->menu.textureTextBox, NULL, &game->menu.textBoxRect);

    if (game->menu.cursorBlink == true && game->menu.textBoxClick == true)
    {
        game->menu.delay++;
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0);
        //Lägg till delay
        if (game->menu.delay == 30)
        {
            game->menu.cursorBlink = false;
            game->menu.delay = 0;
        }
    }
    else if (game->menu.cursorBlink == false && game->menu.textBoxClick == true)
    {
        game->menu.delay++;
        SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
        //Lägg till delay
        if (game->menu.delay == 30)
        {
            game->menu.cursorBlink = true;
            game->menu.delay = 0;
        }
    }

    SDL_RenderFillRect(game->renderer, &game->menu.textCursorRect);
    SDL_RenderDrawRect(game->renderer, &game->menu.textCursorRect);

    FC_DrawColor(font->openSans, game->renderer, 110, 365, FC_MakeColor(255, 255, 255, 255), game->menu.textInTextBox);
    FC_DrawColor(font->openSans, game->renderer, 10, 365, FC_MakeColor(255, 255, 255, 255), "Enter IP:");
}

void HandleMenuEvents(SDL_Event *event)
{
    Game *game = GetGame();
    if (event->type == SDL_KEYDOWN && game->menu.textBoxClick == true)
    {
        SDL_StartTextInput();
        SDL_Event inputEvent;

        while (SDL_PollEvent(&inputEvent))
        {
            if (strlen(game->menu.textInTextBox)<20)
            {
                if (inputEvent.type == SDL_TEXTINPUT)
                {
                    /* Add new text onto the end of our text */
                    game->menu.textLength++;
                    game->menu.textInTextBox[game->menu.textLength] += *inputEvent.text.text;

                    if(game->menu.textInTextBox[game->menu.textLength]== '.'){
                        game->menu.textCursorRect.x+=5;
                    }else
                    {
                        game->menu.textCursorRect.x += 12;
                    }
                    
                }
                else if (inputEvent.key.keysym.sym == SDLK_v && SDL_GetModState() && KMOD_CTRL)
                { //Handle paste
                    game->menu.textCursorRect.x = 110;
                    strcpy(game->menu.textInTextBox, SDL_GetClipboardText());
                    game->menu.textCursorRect.x += 12 * strlen(game->menu.textInTextBox)-25;
                    game->menu.textLength = strlen(game->menu.textInTextBox)-1 ;

                }
                else if (inputEvent.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                { //Handle copy
                    SDL_SetClipboardText(game->menu.textInTextBox);
                }
            }
        }
    }
    else if (event->key.keysym.sym == SDLK_BACKSPACE && game->menu.textLength >= 0)
    {
        game->menu.textInTextBox[game->menu.textLength] = '\0';
        game->menu.textLength--;

        if(game->menu.textInTextBox[game->menu.textLength] =='.')
        {
            game->menu.textCursorRect.x -= 5;
        }else
        {
            game->menu.textCursorRect.x -= 12;
        }
        
        if(game->menu.textCursorRect.x<110)
        {
            game->menu.textCursorRect.x = 110;
        }
    }
    else if (event->key.keysym.sym == SDLK_RETURN)
    {
        GameNetConnect();
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&game->menu.mouseX, &game->menu.mouseY);
        //If the mouse is over the textbox
        if ((game->menu.mouseX > game->menu.textBoxRect.x) && (game->menu.mouseX < game->menu.textBoxRect.x + game->menu.textBoxRect.w) && (game->menu.mouseY > game->menu.textBoxRect.y) && (game->menu.mouseY < game->menu.textBoxRect.y + game->menu.textBoxRect.h))
        {
            game->menu.textBoxClick = true;
        }
    }
}


