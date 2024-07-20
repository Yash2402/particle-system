#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "particle.h"

int main(int argc, char *argv[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Rect r{10, 10, 250, 250};

    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
    while(running){
        while(SDL_PollEvent(&e))
        { 
            if (e.type == SDL_QUIT) running = false; 
            if (e.type == SDL_MOUSEMOTION) { SDL_GetMouseState(&r.x, &r.y); r.x -= 250/2; r.y -= 250/2; }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &r);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    return 0;
}
