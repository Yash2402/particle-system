#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <thread>
#include <mutex>
#include "particle.h"

#define SDL_HINT_RENDER_VSYNC "SDL_RENDER_VSYNC"             
#define WIDTH                  1200
#define HEIGHT                 1200
#define GRID_WIDTH             60
#define RADIUS                 10
#define CONSTRAINT_RADIUS      400
#define FPS                    1000
#define STRENGTH               200

int main(int argc, char *argv[]){
    std::srand(static_cast<unsigned int>(std::time(0)));
    size_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    int NO_OF_PARTICLE = 80;
    double period = (1.0 / (double)FPS)*1000;
    int milliPeriod = (int)period;
    int sleep;

    SDL_Window* window = SDL_CreateWindow("Particle System", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    std::vector<Particle> particles;
    
    for (int i = 0; i < NO_OF_PARTICLE; i++){
        particles.emplace_back(
                rand()%WIDTH,
                rand()%HEIGHT,
                RADIUS, 
                100+rand()%155,
                100+rand()%155,
                100+rand()%155,
                255
                );
    }

    Uint32 lastTick;
    Uint32 currentTick;
    bool update = true;

    while(running){
        while(SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) running = false; 
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) update = !update;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                particles.emplace_back((double)x, (double)y, RADIUS, 100+rand()%155, 100+rand()%155, 100+rand()%155, 255);
                NO_OF_PARTICLE++;
            }
        }
        lastTick = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 100);
        SDL_RenderClear(renderer);
        //  SDL_SetRenderDrawColor(renderer, 30, 30, 30, 0);
        //  filledCircleRGBA(renderer, x, y, CONSTRAINT_RADIUS, 20, 20, 20, 255);
        for (int i = 1; i < (int)WIDTH/GRID_WIDTH; i++){
            lineRGBA(renderer, GRID_WIDTH, GRID_WIDTH*i, HEIGHT - GRID_WIDTH, GRID_WIDTH*i, 80, 80, 80, 50);
            lineRGBA(renderer, GRID_WIDTH*i, GRID_WIDTH,  GRID_WIDTH*i, HEIGHT - GRID_WIDTH, 80, 80, 80, 50);
            if (i%2 == 1){
            lineRGBA(renderer, GRID_WIDTH, GRID_WIDTH*i, HEIGHT - GRID_WIDTH, GRID_WIDTH*i, 80, 80, 80, 100);
            lineRGBA(renderer, GRID_WIDTH*i, GRID_WIDTH,  GRID_WIDTH*i, HEIGHT - GRID_WIDTH, 80, 80, 80, 100);
            }
        }

        size_t chunk_size = particles.size() / num_threads;
        threads.clear();
        for(int i = 0; i < NO_OF_PARTICLE; i++){
            if (update) {
                //particles[i].applyForce(rand()%STRENGTH*pow(-1, rand()%2), rand()%STRENGTH*pow(-1, rand()%2));
                particles[i].applyForce(2, 3000);
                particles[i].edge(WIDTH, HEIGHT, GRID_WIDTH);
                particles[i].update(0.02f);
            }
            particles[i].show(renderer);
            //  int X, Y;
            //  SDL_GetMouseState(&X, &Y);
            //  double dist = pow(pow(particles[i].x - X, 2) + pow(particles[i].y - Y, 2), 0.5);
            //  if (update && dist < CONSTRAINT_RADIUS) {
            //      particles[i].steer(X, Y);
            //      particles[i].update(0.2f);
            //      particles[i].edge(WIDTH, HEIGHT, GRID_WIDTH);
            //  }
        }
        for (int i = 0; i < NO_OF_PARTICLE; i++){
            for (int j = 0; j < NO_OF_PARTICLE; j++){
                if (update && &particles[i] != &particles[j]){
                    particles[j].collision(&particles[i], 15);
                }
            }
        }

        
        SDL_RenderPresent(renderer);

        currentTick = SDL_GetTicks();
        sleep = milliPeriod - (currentTick - lastTick);
        if(sleep < 0) { sleep = 0; }
        SDL_Delay(sleep);
    }
    return 0;
}
