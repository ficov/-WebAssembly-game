#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Snake.hpp"
#include "Food.hpp"
#include "Walls.hpp"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum GameState {
    PLAY,
    PAUSED,
    MENU,
    WINNER,
    DEAD,
    MINUS_LIFE,
    LEVEL_UP
};

struct GameElements {
    SDL_Renderer *renderer;
    SDL_Texture *iconTexture;
    TTF_Font *font;
    int score = 0;
    int totalScore = 0;
    bool running = true;
    int dir = 0;
    int lives = 3;
    int level = 1;
    Snake snake;
    Food apple;
    Walls walls;
    GameState state;
};
