#include "Food.hpp"
#include <random>

//konstruktor
Food::Food() {
    food.w = 40;
    food.h = 40;
}

//generiranje hrane
SDL_Rect Food::generateFood(int width, int height, SDL_Rect& head, SDL_Rect top_wall, SDL_Rect bottom_wall, SDL_Rect left_wall, SDL_Rect right_wall) {
    int wallWidth = 20;
    int wallHeight = 20;
    while (true) {
        food.x = wallWidth + (1 + rand() % ((width - 2*wallWidth) / 10 - 1)) * 10;
        food.y = wallHeight + (1 + rand() % ((height - 2*wallHeight) / 10 - 1)) * 10;
        if (food.x == head.x && food.y == head.y) {
            continue;
        }
        if (SDL_HasIntersection(&food, &top_wall) || SDL_HasIntersection(&food, &bottom_wall) || SDL_HasIntersection(&food, &left_wall) || SDL_HasIntersection(&food, &right_wall)) {
            continue;
        }
        break;
    }
    return food;
}

void Food::renderFood(SDL_Renderer* renderer, SDL_Texture *iconTexture) {
    SDL_RenderCopy(renderer, iconTexture, NULL, &food);
}