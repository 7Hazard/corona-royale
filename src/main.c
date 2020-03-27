#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "collison.h"
#include "events.h"
#include "player.h"



int running = 1;
bool pressed_w = false;
bool pressed_s = false;
bool pressed_a = false;
bool pressed_d = false;
bool noMovement = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


int main(int argc, const char *argv[])
{
    printf("Corona Royale\n");

    if(SDL_Init(0) != 0)
    {
        printf("Could not initialize SDL2");
    }
    
    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
    int initted=IMG_Init(flags);
    if((initted&flags) != flags) 
    {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    // handle error
    }

    int frameWidth,frameHeight;
    int textureWidth,textureHeight;
    int frameTime = 0;
    //SDL_Surface* surface = IMG_Load("C:/Users/Moham/Documents/hi1038-project/res/User.png");

    SDL_Texture* currentImage;

    window = SDL_CreateWindow("Corona Royale",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,800,0);
    SDL_SetWindowBordered(&window,SDL_TRUE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //surface = IMG_Load("res/User.png");
    
    currentImage = IMG_LoadTexture(renderer,"C:/Users/Moham/Documents/hi1038-project/res/User.png");
    
    
    //SDL_FreeSurface(surface);
    SDL_QueryTexture(currentImage,NULL,NULL,&textureWidth,&textureHeight);
    frameHeight = textureHeight/4;
    frameWidth = textureWidth/4;


    Player player;
    CreatePlayer(&player);
    player.playerPosition.x = 10;
    player.playerPosition.y = 10;
    player.rect.w = frameWidth;
    player.rect.h = frameHeight;
    player.playerPosition.w = frameWidth;
    player.playerPosition.h = frameHeight;
    player.infected = true;

    player.rect.x = player.rect.y = 0;

    Player player2;
    CreatePlayer(&player2);
    player2.playerPosition.x = 300;
    player2.playerPosition.y = 300;
    player2.rect.w = 80;
    player2.rect.h = 80;

    player2.rect.x = player2.rect.y = 50;


    while (running)
    {
        if(pressed_w == true)
        {
            player.playerPosition.y-=1;
            player.rect.y = frameHeight;
        }
        else if(pressed_s == true)
        {
            player.playerPosition.y+=1;
            player.rect.y = 0;
        }
        else if(pressed_a == true)
        {
            player.playerPosition.x-=1;
            player.rect.y = frameHeight*2;
        }
        else if(pressed_d == true)
        {
            player.playerPosition.x+=1;
            player.rect.y = frameHeight*3;
        }
        else
        {
            if (player.rect.y == frameHeight*3)
            {
                player.rect.x = frameWidth*3;
            }
            else
            {
                player.rect.x = 0;

            }
            
            
        }
        
        
        

        HandleEvents();
        HandleBorders(&player.playerPosition);
        HandleBorders(&player2.playerPosition);

        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        frameTime++;
        if (frameTime == 50 )
        {
            frameTime = 0;
            player.rect.x += frameWidth;
            if (player.rect.x >= textureWidth)
            {
                player.rect.x = 0;
            }
            
        }
        
        /*
        if(player.infected)
        {
            SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        //SDL_RenderFillRect(renderer,&player.rect);
        if(player2.infected)
        {
            SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
        }
        else {+

            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        SDL_RenderFillRect(renderer,&player2.playerPosition);

        if(DoBoxesIntersect(&player.playerPosition, &player2.playerPosition))
        {
            player2.infected = true;
        }
        */
        SDL_RenderClear(renderer);

        //SDL_RenderFillRect(renderer,&player2.playerPosition);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderCopy(renderer,currentImage,&player.rect,&player.playerPosition);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000/400);
    }
    SDL_DestroyTexture(currentImage);
    IMG_Quit();
    SDL_Quit();
    return 0;
}

