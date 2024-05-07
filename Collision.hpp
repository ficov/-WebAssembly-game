#pragma once

#include <SDL2/SDL.h>
#include "Snake.hpp"

class Collision {
    public:
        static bool selfCollision(Snake& snake);
        static bool wallCollision(SDL_Rect& head, int width, int height);
        static bool foodCollision(SDL_Rect& head, SDL_Rect& food);
};
