#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "collison.h"
#include "events.h"
#include "player.h"
#include "main.h"



int running = 1;

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
    int nrOfPlayers = 2;
    int mapWidth, mapHeight;
    //SDL_Surface* surface = IMG_Load("C:/Users/Moham/Documents/hi1038-project/res/User.png");

    SDL_Texture* currentImage;
    SDL_Texture* background;
    SDL_Rect cameraRect;

    window = SDL_CreateWindow("Corona Royale",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_W,WINDOW_H,0);
    SDL_SetWindowBordered(&window,SDL_TRUE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //surface = IMG_Load("res/User.png");
    
    currentImage = IMG_LoadTexture(renderer,"C:/Users/Moham/Documents/hi1038-project/res/User.png");
    background = IMG_LoadTexture(renderer,"C:/Users/Moham/Documents/hi1038-project/res/background.jpg");
    
    //SDL_FreeSurface(surface);
    SDL_QueryTexture(currentImage,NULL,NULL,&textureWidth,&textureHeight);
    SDL_QueryTexture(background,NULL,NULL,&mapWidth,&mapHeight);

    frameHeight = textureHeight/4;
    frameWidth = textureWidth/4;

    cameraRect.w = WINDOW_W;
    cameraRect.h = WINDOW_H;
    cameraRect.x = 0;
    cameraRect.y = 0;


    Player player;
    CreatePlayer(&player,10,10,frameWidth,frameHeight);

    Player player2;
    CreatePlayer(&player2,100,100,frameWidth,frameHeight);


    while (running)
    {
        
        if(player.up == true)
        {
            player.playerPosition.y-=1;
            player.rect.y = frameHeight;
        }
        else if(player.down == true)
        {
            player.playerPosition.y+=1;
            player.rect.y = 0;
        }
        else if(player.left == true)
        {
            player.playerPosition.x-=1;
            player.rect.y = frameHeight*2;
        }
        else if(player.right == true)
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


        if (player.playerPosition.x >= WINDOW_W/2)
        {
            cameraRect.x = player.playerPosition.x - WINDOW_W/2;
        }
        if (cameraRect.x + cameraRect.w >= mapWidth)
        {
            cameraRect.x = mapWidth-WINDOW_W;
        }
        
        if (player.playerPosition.y >= WINDOW_H/2)
        {
            cameraRect.y = player.playerPosition.y - WINDOW_H/2;
        }
        if (cameraRect.y + cameraRect.h >= mapHeight)
        {
            cameraRect.y = mapHeight-WINDOW_H;
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,background,&cameraRect,NULL);
        if (player.noMovement == false)
        {
            
            
            HandleEvents(&player);
            HandleBorders(&player.playerPosition, frameHeight,frameWidth);
            HandleBorders(&player2.playerPosition, frameHeight,frameWidth);

            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            if (frameTime == 60)
            {
                frameTime = 0;
                player.rect.x += frameWidth;
                if (player.rect.x >= textureWidth)
                {
                    player.rect.x = 0;
                }
                
            }

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_RenderCopy(renderer,currentImage,&player.rect,&player.playerPosition);
            SDL_RenderCopy(renderer,currentImage,&player2.rect,&player2.playerPosition);
            

            SDL_RenderPresent(renderer);
            frameTime++;
            SDL_Delay(1000/400);

        }
        

    }
    SDL_DestroyTexture(currentImage);
    SDL_DestroyTexture(background);
    IMG_Quit();
    SDL_Quit();
    return 0;
}

