#pragma once // kako bi se sprijecilo ukljucivanje vise puta

#include <SDL2/SDL.h>

class Food {
    public:
        Food();
        SDL_Rect generateFood(int width, int height, SDL_Rect& head, SDL_Rect top_wall, SDL_Rect bottom_wall, SDL_Rect left_wall, SDL_Rect right_wall);
        SDL_Rect& getFood() { return food; }
        void setFood(const SDL_Rect& newFood) { food = newFood; }
        void renderFood(SDL_Renderer* renderer, SDL_Texture *iconTexture);
    private:
        SDL_Rect food;
};