#pragma once

#include <SDL2/SDL.h>
#include <deque>    

struct Part {
    SDL_Rect rect;
    int dir;
    int prev_dir;
};

class Snake {
    private:
        SDL_Rect head;
        std::deque<Part> body;
        int snake_size;
        int snake_speed;
    public:
        Snake();
        void grow();
        void reset(bool resetSpeed);
        void renderHead(SDL_Renderer* renderer, SDL_Texture* spriteSheetTexture, int dir);
        void renderBody(SDL_Renderer* renderer, SDL_Texture* spriteSheetTexture, int dir);
        void move(int dir);
        void increaseSpeed();
        int getSpeed();
        std::deque<Part>& getBody();
        SDL_Rect& getHead();
};