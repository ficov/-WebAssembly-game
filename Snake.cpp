#include "Snake.hpp"
#include <algorithm>

//konstruktor
Snake::Snake() : snake_size(1), snake_speed(5), head({500, 500, 10, 10}) {
    body.push_front(head);
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
    body.push_front(head); // kako bi se zmija kretala
    while (body.size() > snake_size) {
        body.pop_back(); //makni visak tijela zmije
    }
}

void Snake::grow() {
    snake_size = snake_size + 5;
    snake_speed++;
}

void Snake::reset() {
    snake_size = 1;
    snake_speed = 5;
    body.clear();
    head.x = 500;
    head.y = 500;
}

std::deque<SDL_Rect>& Snake::getBody() {
    return body;
}

SDL_Rect& Snake::getHead() {
    return head;
}

