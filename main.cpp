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
#define HEIGHT 1200
#define OFFSET 10
#define RADIUS 2
#define NO_OF_PARTICLE 2000

int main(int argc, char *argv[])
{
    SDL_Window* window = SDL_CreateWindow("Particle System", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;

    std::vector<Particle> particles;
    for (int i = 0; i < NO_OF_PARTICLE; i++){
        particles.emplace_back(rand()%(WIDTH-2*OFFSET-2*RADIUS)+OFFSET+RADIUS, rand()%(HEIGHT-2*OFFSET-2*RADIUS)+OFFSET+RADIUS, RADIUS, rand()%200+55, rand()%200+55, rand()%200+55, rand()%200+55);
    }
    bool update = true;
    while(running){
        while(SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) running = false; 
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) update = !update;
        }

        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 100);
        SDL_RenderClear(renderer);

        for(int i = 0; i < NO_OF_PARTICLE; i++){
            particles[i].show(renderer);
            particles[i].applyForce(rand()%20*pow(-1, rand()%2), rand()%20*pow(-1, rand()%2));
            if(update) particles[i].update(1.0f);
            particles[i].edge(WIDTH, HEIGHT, OFFSET);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    return 0;
}
