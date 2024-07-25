#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "quadtree.h"
#include "particle.h"

#define WIDTH                  2560
#define HEIGHT                 1600
#define GRID_WIDTH             40
#define CONSTRAINT_RADIUS      400
#define FPS                    4000
#define STRENGTH               200


int main(int argc, char *argv[]){

    double delta_time = 0.05;
    int RADIUS = 10;
    int steps = 12;
    SDL_Color white = {255, 255, 255, 255};

    double period = (1.0 / (double)FPS)*1000;
    int milliPeriod = (int)period;
    int sleep;

    SDL_Window* window = SDL_CreateWindow("Particle System", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_FULLSCREEN);
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
                for(int i = 0; i < 60; ++i){
                    particles.emplace_back(particles.size()+1, (double)x+i*RADIUS*2, (double)y, RADIUS, 100+rand()%155, 100+rand()%155, 100+rand()%155, 255);
                }
            }
        }

        lastTick = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 100);
        SDL_RenderClear(renderer);
        roundedRectangleRGBA(renderer, 0, 0, WIDTH-2, HEIGHT-2, 16, 200, 200, 200, 255);
        
        for (int i = 1; i < (int)WIDTH/GRID_WIDTH; i++){
            lineRGBA(renderer, GRID_WIDTH, GRID_WIDTH*i, WIDTH - GRID_WIDTH, GRID_WIDTH*i, 110, 110, 110, 50);
            lineRGBA(renderer, GRID_WIDTH*i, GRID_WIDTH,  GRID_WIDTH*i, HEIGHT - GRID_WIDTH, 110, 110, 110, 50);
            if (i%2 == 1){
                lineRGBA(renderer, GRID_WIDTH, GRID_WIDTH*i, WIDTH - GRID_WIDTH, GRID_WIDTH*i, 110, 110, 110, 100);
                lineRGBA(renderer, GRID_WIDTH*i, GRID_WIDTH,  GRID_WIDTH*i, HEIGHT - GRID_WIDTH, 110, 110, 110, 100);
            }
        }

        // Initialize quadtree
        SDL_Rect boundary = {0, 0, WIDTH, HEIGHT};
        Quadtree quadtree(boundary, 2);

        // Insert particles into the quadtree
        for(auto& particle : particles) {
            quadtree.insert(&particle);
        }

        // Show the quadtree
        // quadtree.show(renderer);

        // Update and display particles
        for(size_t i = 0; i < particles.size(); i++){
            if (update) {
                particles[i].applyForce(rand()%STRENGTH*pow(-1, rand()%2), rand()%STRENGTH*pow(-1, rand()%2));
                // particles[i].applyForce(2, 5000);
                particles[i].update(delta_time);
            }
            particles[i].edge(WIDTH, HEIGHT, GRID_WIDTH);
            particles[i].show(renderer);
        }
        
        for (size_t i = 0; i < particles.size(); i++){
            std::vector<Particle*> nearbyParticles;
            SDL_Rect range = {static_cast<int>(particles[i].x) - RADIUS*2, static_cast<int>(particles[i].y) - RADIUS*2, RADIUS*4, RADIUS*4};
            quadtree.query(range, nearbyParticles);
            for (auto& p : nearbyParticles){
                if (update && &particles[i] != p){
                    p->collision(&particles[i], steps);
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

