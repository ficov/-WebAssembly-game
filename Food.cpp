#include "Food.hpp"
#include <random>

//konstruktor
Food::Food() {
    food.w = 40;
    food.h = 40;
}

//generiranje hrane
SDL_Rect Food::generateFood(int width, int height) {
    food.x = rand()% (width / 10) * 10;
    food.y = rand()% (height / 10) * 10;
    return food;
}