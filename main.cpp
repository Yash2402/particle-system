#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "particle.h"

#define WIDTH                  1200
#define HEIGHT                 1200
#define GRID_WIDTH             40
#define RADIUS                 10
#define CONSTRAINT_RADIUS      400
#define FPS                    120
#define STRENGTH               200

std::ostream &operator<<(std::ostream &os, Particle const &particle) { 
    return os <<"id: "<<particle.id<<" : "<<std::endl<<"pos: ("<< particle.x<<", "<<particle.y<<")"<<std::endl<<"vel: ("<<particle.x - particle.prev_x<<", "<<particle.y - particle.prev_y<<")"<<std::endl<<"radius: "<<particle.radius<<std::endl; 
}
int main(int argc, char *argv[]){

    double delta_time = 0.05;
    int steps = 20;
    int NO_OF_PARTICLE = 0;
    SDL_Color white = {255, 255, 255, 255};

    double period = (1.0 / (double)FPS)*1000;
    int milliPeriod = (int)period;
    int sleep;

    SDL_Window* window = SDL_CreateWindow("Particle System", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("./Satoshi_Variable.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    std::vector<Particle> particles;
    for (int i = 0; i < NO_OF_PARTICLE; i++){ particles.emplace_back(i, rand()%WIDTH, rand()%HEIGHT, RADIUS, 100+rand()%155, 100+rand()%155, 100+rand()%155, 255); }

    Uint32 lastTick;
    Uint32 currentTick;
    bool running = true;
    bool update = true;

    while(running){
        while(SDL_PollEvent(&e)) { 
            if (e.type == SDL_QUIT) running = false; 
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) update = !update;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) particles.clear();
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                for(int i = 0; i < 10; ++i){
                    particles.emplace_back(particles.size()+1, (double)x+i*RADIUS*2, (double)y, RADIUS, 100+rand()%155, 100+rand()%155, 100+rand()%155, 255);
                }
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

        /*
        int X, Y;
        SDL_GetMouseState(&X, &Y);
        if (!update){
            aacircleRGBA(renderer, X, Y, CONSTRAINT_RADIUS, 100, 100, 100, 140);
        }
        */

        for(size_t i = 0; i < particles.size(); i++){
            if (update) {
                //particles[i].applyForce(rand()%STRENGTH*pow(-1, rand()%2), rand()%STRENGTH*pow(-1, rand()%2));
                particles[i].applyForce(2, 5000);
                particles[i].update(delta_time);
            }
            particles[i].edge(WIDTH, HEIGHT, GRID_WIDTH);
            particles[i].show(renderer);
        }
        
        for (size_t i = 0; i < particles.size(); i++){
            for (size_t j = i + 1; j < particles.size(); j++){
                if (update && &particles[i] != &particles[j]){
                    particles[j].collision(&particles[i], steps);
                }
            }
        }

        std::string nopText = "NO OF PARTICLE: " + std::to_string(static_cast<int>(particles.size()));
        SDL_Surface *nopSurface = TTF_RenderText_Solid(font, nopText.c_str(), white);
        SDL_Texture *nopTexture = SDL_CreateTextureFromSurface(renderer, nopSurface);

        int textWidth = nopSurface->w;
        int textHeight = nopSurface->h;
        SDL_FreeSurface(nopSurface);

        SDL_Rect renderQuad = {GRID_WIDTH+20, GRID_WIDTH+20, textWidth, textHeight};
        SDL_RenderCopy(renderer, nopTexture, nullptr, &renderQuad);
        SDL_DestroyTexture(nopTexture);

        SDL_RenderPresent(renderer);

        currentTick = SDL_GetTicks();
        sleep = milliPeriod - (currentTick - lastTick);
        if (sleep < 0) sleep = 0;
        SDL_Delay(sleep);
    }
    return 0;
}


/*TODO::Parallelize this bitch*/
/*
   size_t chunk_size = particles.size() / num_threads;
   threads.clear();

   for (size_t i = 0; i < num_threads; ++i) {
   size_t start = i * chunk_size;
   size_t end = (i == num_threads - 1) ? particles.size() : start + chunk_size;
   threads.emplace_back(Particle::update, &particles, delta_time, start, end);
   }
   */
/*
double dist = pow(pow(particles[i].x - X, 2) + pow(particles[i].y - Y, 2), 0.5);
if (!update && dist < CONSTRAINT_RADIUS) { 
    particles[i].steer(X, Y); 
    for (size_t j = 0; j < particles.size(); j++){
        if (&particles[i] != &particles[j]){ particles[j].collision(&particles[i], steps); }
    }
}
*/
