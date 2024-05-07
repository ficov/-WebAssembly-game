#include "Collision.hpp"
#include <algorithm>

bool Collision::selfCollision(Snake& snake) {
    auto body = snake.getBody();
    //krece od drugog dijela tijela zmije jer je prvi dio glava zmije
    for (auto snake_part = body.begin() + 1; snake_part != body.end(); ++snake_part) {
        if (snake_part->x == snake.getHead().x && snake_part->y == snake.getHead().y) {
            return true;
        }
    }
    return false;
}

bool Collision::wallCollision(SDL_Rect& head, int width, int height) {
    if (head.x < 0 || head.x >= width || head.y < 0 || head.y >= height) {
        return true;
    }
    return false;
}

bool Collision::foodCollision(SDL_Rect& head, SDL_Rect& food) {
    //kako bi radijus hrane bio veci
    if ((head.x - food.x)*(head.x - food.x) + (head.y - food.y)*(head.y - food.y) < 100) {
        return true;
    }
    return false;
}

