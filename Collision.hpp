#pragma once

#include <SDL2/SDL.h>
#include "Snake.hpp"

class Collision {
    public:
        static bool selfCollision(Snake& snake);
        static bool wallCollision(SDL_Rect& head, SDL_Rect top_wall, SDL_Rect bottom_wall, SDL_Rect left_wall, SDL_Rect right_wall);
        static bool foodCollision(SDL_Rect& head, SDL_Rect& food);
};
