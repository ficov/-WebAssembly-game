#include "Snake.hpp"
#include <algorithm>
#include <iostream>

//konstruktor
Snake::Snake() : snake_size(15), snake_speed(6), head({465, 420, 40, 40}) {
    body.push_front({head, 0, 0});
}

void Snake::move(int direction) {
    switch (direction) {
        case 0: // UP
            head.y -= snake_speed;
            break;
        case 1: // DOWN
            head.y += snake_speed;
            break;
        case 2: // LEFT
            head.x -= snake_speed;
            break;
        case 3: // RIGHT
            head.x += snake_speed;
            break;
    }
    body.push_front({head, direction, body.front().dir}); // kako bi se zmija kretala
    while (body.size() > snake_size) {
        body.pop_back(); //makni visak tijela zmije
    }
}

void Snake::grow() {
    snake_size = snake_size + 5;
}

void Snake::increaseSpeed() {
    snake_speed += 3;
}

int Snake::getSpeed() {
    return snake_speed;
}

void Snake::reset(bool resetSpeed) {
    snake_size = 10;
    if (resetSpeed) {
        snake_speed = 6;
    }
    body.clear();
    head.x = 475;
    head.y = 450;
}

std::deque<Part>& Snake::getBody() {
    return body;
}

SDL_Rect& Snake::getHead() {
    return head;
}

void Snake::renderHead(SDL_Renderer* renderer, SDL_Texture* spriteSheetTexture, int dir) {
    SDL_Rect srcRect;
     switch (dir) {
        case 0: // UP
            srcRect = {192, 0, 60, 65};
            break;
        case 1: // DOWN
            srcRect = {258, 64, 61, 63};
            break;
        case 2: // LEFT
            srcRect = {195, 65, 64, 61};
            break;
        case 3: // RIGHT
            srcRect = {255, 0, 66, 64};
            break;
    }
    SDL_RenderCopy(renderer, spriteSheetTexture, &srcRect, &head);
}

void Snake::renderBody(SDL_Renderer* renderer, SDL_Texture* spriteSheetTexture, int dir) {
    SDL_Rect srcRect;
    for (auto it = body.begin(); it != body.end(); ++it) {
        SDL_Rect destRect = {it->rect.x, it->rect.y, 40, 40};
        if (it == body.begin() || it == std::next(body.begin()) || it == (body.end() - 2)){
            continue;
        }
        if (it->prev_dir == it->dir) {
            switch (it->dir) {
                case 0: // UP
                    if (it == body.end() - 1) {
                        srcRect = {198, 127, 51, 59};
                    } else {
                        srcRect = {135, 57, 48, 35};
                    }
                    break;
                case 1: // DOWN
                    if (it == body.end() - 1) {
                        srcRect = {260, 192, 56, 64};
                    } else {
                        srcRect = {135, 57, 48, 35};
                    }
                    break;
                case 2: // LEFT
                    if (it == body.end() - 1) {
                        srcRect = {190, 196, 64, 58};
                    } else {
                        srcRect = {61, 7, 54, 47};
                    }
                    break;
                case 3: // RIGHT
                    if (it == body.end() - 1) {
                        srcRect = {263, 135, 30, 52};
                    } else {
                        srcRect = {61, 7, 54, 47};
                    }
                    break;
            }
        } else {
            switch(it->prev_dir*10 + it->dir) {
                case 31: //RIGHT -> DOWN
                    srcRect = {137, 7, 49, 57};
                    break;
                case 2: //UP -> LEFT
                    srcRect = {137, 7, 49, 57};
                    break;
                case 3: //UP -> RIGHT
                    srcRect = {9, 7, 51, 64};
                    break;
                case 21: //LEFT -> DOWN
                    srcRect = {9, 7, 51, 64};
                    break;
                case 30: //RIGHT -> UP
                    srcRect = {126, 116, 60, 72};
                    break;
                case 12: //DOWN -> LEFT
                    srcRect = {126, 116, 60, 72};
                    break;
                case 13: //DOWN -> RIGHT    
                    srcRect = {6, 57, 60, 66};
                    break;
                case 20: //LEFT -> UP
                    srcRect = {6, 57, 60, 66};
                    break;
            }
        }
        SDL_RenderCopy(renderer, spriteSheetTexture, &srcRect, &destRect);
    }
}
