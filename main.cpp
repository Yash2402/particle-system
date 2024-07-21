#include <iostream>
#include <vector>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "particle.h"
#define SDL_HINT_RENDER_VSYNC "SDL_RENDER_VSYNC"             
#define WIDTH 1200
#define HEIGHT 900
#define OFFSET 10

int main(int argc, char *argv[])
{
    SDL_Window* window = SDL_CreateWindow("Particle System", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;

    std::vector<Particle> particles;
    for (int i = 0; i < 100; i++){
        particles.emplace_back(rand()%WIDTH, rand()%HEIGHT, 0, 0, 10, rand()%255, rand()%255, rand()%255, rand()%255);
    }
    bool update = true;
    while(running){
        while(SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) running = false; 
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) update = !update;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
        SDL_RenderClear(renderer);

        for(int i = 0; i < 100; i++){
            particles[i].show(renderer);
            particles[i].applyForce(0, rand()%5);
            if(update) particles[i].update();
            particles[i].edge(WIDTH, HEIGHT, OFFSET);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    return 0;
}
