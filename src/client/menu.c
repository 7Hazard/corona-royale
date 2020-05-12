
#include "menu.h"
#include "game.h"
#include "fonts.h"
#include "textures.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <SDL_FontCache.h>
#include <SDL_image.h>

Menu *GetMenu()
{
    Game *game = GetGame();
    static Menu menu;
    static bool inited = false;
    //Menu menu;
    if (inited == false)
    {
        inited = true;
        menu.textBoxClick = false;
        menu.cursorBlink = false;
        menu.delay = 0;

        menu.textLength = -1;

        menu.logoRect.x = 50;
        menu.logoRect.y = 30;
        menu.logoRect.w = 320;
        menu.logoRect.h = 250;

        menu.serverTextRect.x = 50;
        menu.serverTextRect.y = 30;
        menu.serverTextRect.w = 50;
        menu.serverTextRect.h = 20;

        menu.textBoxRect.x = 100;
        menu.textBoxRect.y = 340;
        menu.textBoxRect.h = 80;
        menu.textBoxRect.w = 250;

        menu.textColor.r = 255;
        menu.textColor.g = 255;
        menu.textColor.b = 255;
        menu.textColor.a = 255;

        menu.boxOutlineColor.r = 0;
        menu.boxOutlineColor.g = 0;
        menu.boxOutlineColor.b = 0;
        menu.boxOutlineColor.a = 0;

        menu.boxBackgroundcolor.r = 0;
        menu.boxBackgroundcolor.g = 0;
        menu.boxBackgroundcolor.b = 0;
        menu.boxBackgroundcolor.a = 0;

        menu.textCursorRect.x = 110;
        menu.textCursorRect.y = 366;
        menu.textCursorRect.w = 1;
        menu.textCursorRect.h = 25;

        menu.textCursorColor.r = 255;
        menu.textCursorColor.g = 255;
        menu.textCursorColor.b = 255;
        menu.textCursorColor.a = 255;
    }

    return &menu;
}

void RenderMenu()
{
    Game *game = GetGame();
    Fonts *font = GetFonts();
    Textures *tex = GetTextures();
    Menu *menu = GetMenu();

    SDL_RenderCopy(game->renderer, tex->textureMenu, NULL, NULL);
    SDL_RenderCopy(game->renderer, tex->textureLogo, NULL, &menu->logoRect);
    SDL_RenderCopy(game->renderer, tex->textureTextBox, NULL, &menu->textBoxRect);

    if (menu->cursorBlink == true && menu->textBoxClick == true)
    {
        menu->delay++;
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0);
        if (menu->delay == 30)
        {
            menu->cursorBlink = false;
            menu->delay = 0;
        }
    }
    else if (menu->cursorBlink == false && menu->textBoxClick == true)
    {
        menu->delay++;
        SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
        if (menu->delay == 30)
        {
            menu->cursorBlink = true;
            menu->delay = 0;
        }
    }

    SDL_RenderFillRect(game->renderer, &menu->textCursorRect);
    SDL_RenderDrawRect(game->renderer, &menu->textCursorRect);

    FC_DrawColor(font->openSans, game->renderer, 110, 365, FC_MakeColor(255, 255, 255, 255), menu->textInTextBox);
    FC_DrawColor(font->openSans, game->renderer, 10, 365, FC_MakeColor(255, 255, 255, 255), "Enter IP:");
}

void HandleMenuEvents(SDL_Event *event)
{
    Game *game = GetGame();
    Menu *menu = GetMenu();

    if (event->type == SDL_KEYDOWN && menu->textBoxClick == true)
    {
        SDL_StartTextInput();
        SDL_Event inputEvent;

        while (SDL_PollEvent(&inputEvent))
        {
            if (strlen(menu->textInTextBox) < 20)
            {
                if (inputEvent.type == SDL_TEXTINPUT)
                {
                    /* Add new text onto the end of our text */
                    menu->textLength++; // sparas inte
                    menu->textInTextBox[menu->textLength] += *inputEvent.text.text;
                    //only small letters
                    menu->textInTextBox[menu->textLength] = tolower(menu->textInTextBox[menu->textLength]);

                    if (menu->textInTextBox[menu->textLength] == '.')
                    {
                        menu->textCursorRect.x += 5;
                    }
                    else if (isalpha(menu->textInTextBox[menu->textLength]) > 0)
                    {
                        if (menu->textInTextBox[menu->textLength] == 'i' || menu->textInTextBox[menu->textLength] == 'l')
                        {
                            menu->textCursorRect.x += 5;
                        }
                        else if (menu->textInTextBox[menu->textLength] == 'm')
                        {
                            menu->textCursorRect.x += 20;
                        }
                        else
                        {
                            menu->textCursorRect.x += 12;
                        }
                    }
                    else if (isdigit(menu->textInTextBox[menu->textLength]) > 0)
                    {
                        menu->textCursorRect.x += 12;
                    }
                }
                else if (inputEvent.key.keysym.sym == SDLK_v && SDL_GetModState() && KMOD_CTRL)
                { //Handle paste
                    menu->textCursorRect.x = 110;
                    strcpy(menu->textInTextBox, SDL_GetClipboardText());
                    menu->textLength = strlen(menu->textInTextBox) - 1;
                    for (int i = 0; i < 20; i++)
                    {
                        if (menu->textInTextBox[i] == '\0')
                        {
                            break;
                        }
                        else
                        {
                            if (menu->textInTextBox[i] == '.')
                            {
                                menu->textCursorRect.x += 5;
                            }
                            else if (isalpha(menu->textInTextBox[i]))
                            {
                                if (menu->textInTextBox[i] == 'i' || menu->textInTextBox[i] == 'l')
                                {
                                    menu->textCursorRect.x += 5;
                                }
                                else
                                {
                                    menu->textCursorRect.x += 12;
                                }
                            }
                            else if (isdigit(menu->textInTextBox[i]))
                            {
                                menu->textCursorRect.x += 12;
                            }
                        }
                    }
                }
                else if (inputEvent.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                { //Handle copy
                    SDL_SetClipboardText(menu->textInTextBox);
                }
            }
        }
    }
    else if (event->key.keysym.sym == SDLK_BACKSPACE && menu->textLength >= 0)
    {
        menu->textInTextBox[menu->textLength] = '\0';
        menu->textLength--;

        if (menu->textInTextBox[menu->textLength] == '.')
        {
            menu->textCursorRect.x -= 5;
        }
        else if (menu->textInTextBox[menu->textLength] == 'm')
        {
            menu->textCursorRect.x -= 20;
        }
        else
        {
            menu->textCursorRect.x -= 12;
        }

        if (menu->textCursorRect.x < 110)
        {
            menu->textCursorRect.x = 110;
        }
        if (menu->textInTextBox[0] == '\0')
        {
            menu->textCursorRect.x = 110;
        }
    }
    else if (event->key.keysym.sym == SDLK_RETURN)
    {
        GameNetConnect();
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&menu->mouseX, &menu->mouseY);
        //If the mouse is over the textbox
        if ((menu->mouseX > menu->textBoxRect.x) && (menu->mouseX < menu->textBoxRect.x + menu->textBoxRect.w) && (menu->mouseY > menu->textBoxRect.y) && (menu->mouseY < menu->textBoxRect.y + menu->textBoxRect.h))
        {
            menu->textBoxClick = true;
        }
    }
}
