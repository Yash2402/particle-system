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
    //  SDL_Rect r{10, 10, 250, 250};

    Particle particle1(320, 240, 10, 0, 10);
    Particle particle2(50, 240, 4, 0, 10);

    std::vector<Particle> particles;
    for (int i = 0; i < 100; i++){
        particles.emplace_back(rand()%WIDTH, rand()%HEIGHT, rand()%10, rand()%10, 10);
    }

    //  SDL_Surface* ball = IMG_Load("./bitmap.png");
    while(running){
        while(SDL_PollEvent(&e))
        { 
            if (e.type == SDL_QUIT) running = false; 
//            if (e.type == SDL_MOUSEMOTION) { SDL_GetMouseState(&particle.x, &particle.y); particle.x -= 250/2; particle.y -= 250/2; }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderClear(renderer);
        for(int i = 0; i < 100; i++){
            particles[i].show(renderer);
            particles[i].applyForce(0, rand()%5);
            particles[i].update();
            particles[i].edge(WIDTH, HEIGHT, OFFSET);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    return 0;
}
