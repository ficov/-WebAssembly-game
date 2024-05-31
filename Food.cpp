#include "Food.hpp"
#include <random>

//konstruktor
Food::Food() {
    food.w = 40;
    food.h = 40;
}

//generiranje hrane
SDL_Rect Food::generateFood(int width, int height) {
    int wallWidth = 20;
    int wallHeight = 20;

    food.x = wallWidth + (1 + rand() % ((width - 2*wallWidth) / 10 - 1)) * 10;
    food.y = wallHeight + (1 + rand() % ((height - 2*wallHeight) / 10 - 1)) * 10;
    return food;
}

void Food::renderFood(SDL_Renderer* renderer, SDL_Texture *iconTexture) {
    SDL_RenderCopy(renderer, iconTexture, NULL, &food);
}