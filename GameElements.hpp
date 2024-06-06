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
    SDL_Texture* spriteSheetTexture;
    SDL_Texture* livesTexture;
    SDL_Texture* levelTexture;
    SDL_Texture* scoreTexture;
    SDL_Texture* speedTexture;
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
    SDL_Rect livesRect = {285, 23, 40, 40};
    SDL_Rect scoreRect = {85, 15, 50, 50};
    SDL_Rect levelRect = {485, 23, 40, 40};
    SDL_Rect speedRect = {680, 23, 40, 40};
};
