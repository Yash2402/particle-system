#pragma once

class Particle{
    public:
        int x, y;
        int radius;
        int velx, vely;
        int accx, accy;

        Particle(int x, int y, int velx, int vely, int accx, int accy, int radius): x(x), y(y), velx(velx), vely(vely), accx(accx), accy(accy), radius(radius){}

        void updateParticle(int deltatime){
            x += velx*deltatime;
            y += vely*deltatime;
            velx += accx*deltatime*deltatime;
            vely += accy*deltatime*deltatime;
            accx = 0; accy = 0;
        }

        void show(SDL_Renderer *renderer){
            SDL_RenderDrawPoint(renderer, x, y);
            filledCircleRGBA(renderer, x, y, radius, 0, 0, 0, 255);
        }
        void edge(int width, int height, int offset){
            if (this->x < offset + this->radius){ this->velx *= -1;}
            if (this->x > width - offset - this->radius){ this->velx *= -1;}
            if (this->y < offset + this->radius){ this->vely *= -1;}
            if (this->y > height - offset - this->radius){ this->vely *= -1;}
        }
        ~Particle(){};
};
