#pragma once

class Particle{
    public:
        double x, y; 
        double prev_x = x;
        double prev_y = y; 
        double accx, accy;
        int radius;
        int mass = 125;
        int drag = 1;
        int r = 0;
        int g = 0;
        int b = 0;
        int a = 255;

        Particle(double x, double y, int radius): x(x), y(y), radius(radius){}
        Particle(double x, double y, int radius, int r, int g, int b, int a): x(x), y(y), radius(radius), r(r), g(g), b(b), a(a){}

        void update(double dt){
            double velx = this->x - this->prev_x;
            double vely = this->y - this->prev_y;
            prev_x = x;
            prev_y = y;
            this->x += velx + accx*dt*dt; 
            this->y += vely + accy*dt*dt; 
        }
        void applyForce(double forcex, double forcey){
            this->accx = forcex/mass;
            this->accy = forcey/mass;
        }

        void show(SDL_Renderer *renderer){
            filledCircleRGBA(renderer, x, y, radius, r, g, b, a);
        }
        void steer(int x, int y){
            double dx = (x - this->x)/20.0f;
            double dy = (y - this->y)/20.0f;
            this->applyForce(dx, dy);
            this->update(0.20f);
        }
        void collision(Particle* particle, int step){
            for(int i = 0; i < step; ++i){
                double dx = this->x - particle->x;
                double dy = this->y - particle->y;
                double dist = pow(pow(dx, 2) + pow(dy, 2), 0.5);
                if (dist < this->radius + particle->radius) {
                    double n_dx = dx/dist;
                    double n_dy = dy/dist;
                    double delta = (this->radius + particle->radius - dist);
                    this->x += delta*n_dx*0.5f;
                    this->y += delta*n_dy*0.5f;
                    particle->x -= delta*n_dx*0.5f;
                    particle->y -= delta*n_dy*0.5f;
                    //  std::cout<<this->x - this->prev_x<<"  "<<this->y - this->prev_y<<std::endl;
                    //  std::cout<<particle->x - particle->prev_x<<"  "<<particle->y - particle->prev_y<<std::endl;
                }

            }
        }
        void circularEdge(double x, double y, double radius) {
            double dx = this->x - x;
            double dy = this->y - y;
            double dist = pow(pow(dx, 2) + pow(dy, 2), 0.5);
            if (dist >= radius - this->radius){
                double n_dx = dx/dist;
                double n_dy = dy/dist;
                this->x = x + n_dx * (dist - this->radius);
                this->y = y + n_dy * (dist - this->radius);
            }
        }

        void edge(int width, int height, int offset){
            double velx = (this->x - this->prev_x)*0.8f;
            double vely = (this->y - this->prev_y)*0.8f;

            if (this->x <= offset + this->radius){ 
                this->x = offset + this->radius; 
                this->prev_x = this->x + velx;
            } if (this->x >= width - offset - this->radius){ 
                this->x = width - offset - this->radius;
                this->prev_x = this->x + velx;
            } if (this->y <= offset + this->radius){ 
                this->y = offset + this->radius;
                this->prev_y = this->y + vely;
            } if (this->y >= height - offset - this->radius){ 
                this->y = height - offset - this->radius;
                this->prev_y = this->y + vely;
            }
        }
        ~Particle(){};
};
