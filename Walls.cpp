#include "Walls.hpp"

Walls::Walls() {
    top_wall = {0, 0, 0, 0};
    bottom_wall = {0, 0, 0, 0};
    left_wall = {0, 0, 0, 0};
    right_wall = {0, 0, 0, 0};
}

void Walls::generateWalls(int width, int height) {
    top_wall = {0, 0, width, 20};
    bottom_wall = {0, height - 20, width, 20};
    left_wall = {0, 0, 20, height};
    right_wall = {width - 20, 0, 20, height};
}

void Walls::renderWalls(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_RenderFillRect(renderer, &top_wall);
    SDL_RenderFillRect(renderer, &bottom_wall);
    SDL_RenderFillRect(renderer, &left_wall);
    SDL_RenderFillRect(renderer, &right_wall);
}