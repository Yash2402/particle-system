#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

class Particle{
    public:
        int x, y;
        int velx, vely;
        int accx, accy;
        int dt;
        
        Particle(int x, int y, int velx, int vely, int accx, int accy): x(x), y(y), velx(velx), vely(vely), accx(accx), accy(accy){};
      //void update(int dt): dt(dt){
      //    x += velx*delta_time;
      //    y += vely*delta_time;
      //    velx += accx*delta_time*delta_time;
      //    vely += accy*delta_time*delta_time;
      //    accx = 0; accy = 0;
      //};
        void show(SDL_Renderer *renderer, SDL_Surface *surface){
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            SDL_Rect particle_rect;
            SDL_QueryTexture(texture, NULL, NULL, &particle_rect.w, &particle_rect.h);
            SDL_RenderClear(renderer); 
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, &particle_rect);
        }
        ~Particle(){};
};
