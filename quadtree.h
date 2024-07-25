#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "particle.h"

class Quadtree {
public:
    SDL_Rect boundary;
    int capacity;
    std::vector<Particle*> particles;
    bool divided;

    Quadtree* northeast;
    Quadtree* northwest;
    Quadtree* southeast;
    Quadtree* southwest;

    Quadtree(SDL_Rect boundary, int capacity) 
        : boundary(boundary), capacity(capacity), divided(false), 
          northeast(nullptr), northwest(nullptr), southeast(nullptr), southwest(nullptr) {}

    ~Quadtree() {
        delete northeast;
        delete northwest;
        delete southeast;
        delete southwest;
    }

    bool insert(Particle* particle) {
        if (!contains(boundary, particle)) {
            return false;
        }

        if ((int)particles.size() < capacity) {
            particles.push_back(particle);
            return true;
        } else {
            if (!divided) {
                subdivide();
            }
            if (northeast->insert(particle)) return true;
            if (northwest->insert(particle)) return true;
            if (southeast->insert(particle)) return true;
            if (southwest->insert(particle)) return true;
        }
        return false;
    }

    void subdivide() {
        int x = boundary.x;
        int y = boundary.y;
        int w = boundary.w / 2;
        int h = boundary.h / 2;

        SDL_Rect ne = {x + w, y, w, h};
        SDL_Rect nw = {x, y, w, h};
        SDL_Rect se = {x + w, y + h, w, h};
        SDL_Rect sw = {x, y + h, w, h};

        northeast = new Quadtree(ne, capacity);
        northwest = new Quadtree(nw, capacity);
        southeast = new Quadtree(se, capacity);
        southwest = new Quadtree(sw, capacity);

        divided = true;
    }

    bool contains(SDL_Rect range, Particle* particle) {
        return (particle->x >= range.x - particle->radius &&
                particle->x < range.x + range.w + particle->radius &&
                particle->y >= range.y - particle->radius &&
                particle->y < range.y + range.h + particle->radius);
    }

    void query(SDL_Rect range, std::vector<Particle*>& found) {
        if (!SDL_HasIntersection(&boundary, &range)) {
            return;
        } else {
            for (Particle* p : particles) {
                if (contains(range, p)) {
                    found.push_back(p);
                }
            }
            if (divided) {
                northeast->query(range, found);
                northwest->query(range, found);
                southeast->query(range, found);
                southwest->query(range, found);
            }
        }
    }

    void clear() {
        particles.clear();
        if (divided) {
            northeast->clear();
            northwest->clear();
            southeast->clear();
            southwest->clear();
            delete northeast;
            delete northwest;
            delete southeast;
            delete southwest;
            northeast = nullptr;
            northwest = nullptr;
            southeast = nullptr;
            southwest = nullptr;
            divided = false;
        }
    }

    void show(SDL_Renderer* renderer) {
        // Draw the boundary of this quadtree node
        rectangleRGBA(renderer, boundary.x, boundary.y, boundary.x + boundary.w, boundary.y + boundary.h, 100, 100, 100, 255);

        // Recursively draw the boundaries of the child nodes
        if (divided) {
            northeast->show(renderer);
            northwest->show(renderer);
            southeast->show(renderer);
            southwest->show(renderer);
        }
    }
};

