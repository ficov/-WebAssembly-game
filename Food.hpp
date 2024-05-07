#pragma once // kako bi se sprijecilo ukljucivanje vise puta

#include <SDL2/SDL.h>

class Food {
    public:
        Food();
        SDL_Rect generateFood(int width, int height);
        SDL_Rect& getFood() { return food; }
        void setFood(const SDL_Rect& newFood) { food = newFood; }
    private:
        SDL_Rect food;
};