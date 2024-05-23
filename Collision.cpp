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

bool Collision::wallCollision(SDL_Rect& head, SDL_Rect top_wall, SDL_Rect bottom_wall, SDL_Rect left_wall, SDL_Rect right_wall) {
    if (SDL_HasIntersection(&head, &top_wall) || SDL_HasIntersection(&head, &bottom_wall) || SDL_HasIntersection(&head, &left_wall) || SDL_HasIntersection(&head, &right_wall)){
        return true;
    }
    return false;
}

bool Collision::foodCollision(SDL_Rect& head, SDL_Rect& food) {
    //kako bi radijus hrane bio veci
    if ((head.x - food.x)*(head.x - food.x) + (head.y - food.y)*(head.y - food.y) < 1600) {
        return true;
    }
    return false;
}

