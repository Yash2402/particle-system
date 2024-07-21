#pragma once

class Particle{
    public:
        float x, y; 
        float prev_x = x;
        float prev_y = y; 
        float accx, accy;
        int radius;
        int mass = 25;
        int r = 0;
        int g = 0;
        int b = 0;
        int a = 255;

        Particle(float x, float y, int radius): x(x), y(y), radius(radius){}
        Particle(float x, float y, int radius, int r, int g, int b, int a): x(x), y(y), radius(radius), r(r), g(g), b(b), a(a){}

        void update(float dt){
            float velx = this->x - this->prev_x;
            float vely = this->y - this->prev_y;
            prev_x = x;
            prev_y = y;
            this->x += velx + accx*dt*dt; 
            this->y += vely + accy*dt*dt; 
        }
        void applyForce(float forcex, float forcey){
            this->accx = forcex/mass;
            this->accy = forcey/mass;
        }

        void show(SDL_Renderer *renderer){
            filledCircleRGBA(renderer, x, y, radius, r, g, b, a);
        }
        void edge(int width, int height, int offset){
            float velx = (this->x - this->prev_x)*0.3f;
            float vely = (this->y - this->prev_y)*0.3f;

            if (this->x <= offset + this->radius){ 
                this->x = offset + this->radius; 
                this->prev_x = this->x + velx;
            } else if (this->x >= width - offset - this->radius){ 
                this->x = width - offset - this->radius;
                this->prev_x = this->x + velx;
            } else if (this->y <= offset + this->radius){ 
                this->y = offset + this->radius;
                this->prev_y = this->y + vely;
            } else if (this->y >= height - offset - this->radius){ 
                this->y = height - offset - this->radius;
                this->prev_y = this->y + vely;
            }
        }
        ~Particle(){};
};
