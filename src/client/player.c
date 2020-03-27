#include "player.h"
#include "events.h"
#include "main.h"
void CreatePlayer(Player* player)
{
    player->steps  = Mix_LoadMUS("res/footstep.mp3");
    player->infected = false;
    player->rect.x = 0;
    player->rect.y = 0;
    player->rect.w = 0;
    player->rect.h = 0;
}
void HandlePlayerEvents(SDL_Event *event)
{
    
    if(event->type == SDL_KEYDOWN)
        {
            // Mix_PlayChannel(-1, walking, 0);
            switch (event->key.keysym.sym)
            {
                case SDLK_a: pressed_a = true;
                break;
                case SDLK_d: pressed_d = true;
                break;
                case SDLK_w: pressed_w = true;
                break;
                case SDLK_s: pressed_s = true;
                break;
            }
        }
    if(event->type == SDL_KEYUP)
    {
        switch (event->key.keysym.sym)
        {
            case SDLK_a: pressed_a = false;
            break;
            case SDLK_d: pressed_d = false;
            break;
            case SDLK_w: pressed_w = false;
            break;
            case SDLK_s: pressed_s = false;
            break;
        }
    }
}