#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "particle.h"

#define SDL_HINT_RENDER_VSYNC "SDL_RENDER_VSYNC"             
#define WIDTH 1200
#define HEIGHT 900
#define OFFSET 10
#define MOUSE_RADIUS 200
#define RADIUS 5
#define NO_OF_PARTICLE 5000

int main(int argc, char *argv[])
{
    SDL_Window* window = SDL_CreateWindow("Particle System", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;

    std::vector<Particle> particles;
    for (int i = 0; i < NO_OF_PARTICLE; i++){
        particles.emplace_back(
                rand()%(WIDTH-2*OFFSET-2*RADIUS)+OFFSET+RADIUS, 
                rand()%(HEIGHT-2*OFFSET-2*RADIUS)+OFFSET+RADIUS, 
                RADIUS, 
                rand()%200+55, 
                rand()%200+55, 
                rand()%200+55, 
                rand()%200+55
                );
    }

    bool update = true;
    while(running){
        while(SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) running = false; 
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) update = !update;
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 10);
        SDL_RenderClear(renderer);

        for(int i = 0; i < NO_OF_PARTICLE; i++){
            particles[i].show(renderer);
            particles[i].applyForce(rand()%20*pow(-1, rand()%2), rand()%20*pow(-1, rand()%2));
            if (update) particles[i].update(0.2f);
            particles[i].edge(WIDTH, HEIGHT, OFFSET);
            int x, y;
            SDL_GetMouseState(&x, &y);
            float dist = pow(pow(particles[i].x - x, 2) + pow(particles[i].y - y, 2), 0.5);
            if (!update && dist < MOUSE_RADIUS) particles[i].steer(x, y);
        }
        SDL_RenderPresent(renderer);
    }
    return 0;
}
