#pragma once

#include <SDL2/SDL.h>
#include <deque>    

class Snake {
    private:
        SDL_Rect head;
        std::deque<SDL_Rect> body;
        int snake_size;
        int snake_speed;
    public:
        Snake();
        void grow();
        void reset();
        void move(int dir);
        void increaseSpeed();
        int getSpeed();
        std::deque<SDL_Rect>& getBody();
        SDL_Rect& getHead();
};