#pragma once

class Particle{
    public:
        int x, y;
        int velx;
        int vely;
        int accx;
        int accy;
        int radius;
        int mass = 1;
        int r = 0;
        int g = 0;
        int b = 0;
        int a = 255;

        Particle(int x, int y, int velx, int vely, int radius): x(x), y(y), velx(velx), vely(vely), radius(radius){}
        Particle(int x, int y, int velx, int vely, int radius, int r, int g, int b, int a): x(x), y(y), velx(velx), vely(vely), radius(radius), r(r), g(g), b(b), a(a){}

        void update(){
            x += velx;
            y += vely;
            velx += accx;
            vely += accy;
            accx = 0; accy = 0;
        }
        void applyForce(int forcex, int forcey){
            this->accx = (int)forcex/mass;
            this->accy = (int)forcey/mass;
        }

        void show(SDL_Renderer *renderer){
            filledCircleRGBA(renderer, x, y, radius, r, g, b, a);
        }
        void edge(int width, int height, int offset){
            if (this->x < offset + this->radius){ this->x = offset + this->radius; this->velx *= -0.9;}
            if (this->x > width - offset - this->radius){ this->x = width - offset - this->radius; this->velx *= -0.9;}
            if (this->y < offset + this->radius){ this->y = offset + this->radius; this->vely *= -0.9;}
            if (this->y > height - offset - this->radius){ this->y = height - offset - this->radius; this->vely *= -0.9;}
        }
        ~Particle(){};
};
