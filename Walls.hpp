#pragma once

#include <SDL2/SDL.h>

class Walls {
    public:
        Walls();
        void renderWalls(SDL_Renderer* renderer);
        void generateWalls(int width, int height);
        SDL_Rect& getTopWall() {return top_wall;}
        SDL_Rect& getBottomWall() {return bottom_wall;}
        SDL_Rect& getLeftWall() {return left_wall;}
        SDL_Rect& getRightWall() {return right_wall;}
    private:
        SDL_Rect top_wall;
        SDL_Rect bottom_wall;
        SDL_Rect left_wall;
        SDL_Rect right_wall;
};