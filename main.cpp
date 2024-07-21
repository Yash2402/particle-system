#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "particle.h"
#define SDL_HINT_RENDER_VSYNC 1             

int main(int argc, char *argv[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Rect r{10, 10, 250, 250};
    Particle particle(320, 240, 2, 0, 0, 10, 20);

    SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
    SDL_Surface* ball = IMG_Load("./bitmap.png");
    while(running){
        while(SDL_PollEvent(&e))
        { 
            if (e.type == SDL_QUIT) running = false; 
//            if (e.type == SDL_MOUSEMOTION) { SDL_GetMouseState(&particle.x, &particle.y); particle.x -= 250/2; particle.y -= 250/2; }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderClear(renderer);
        particle.show(renderer);
        particle.updateParticle(1);
        particle.edge(640, 480, 40);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    return 0;
}
