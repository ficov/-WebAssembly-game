#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Food.hpp"
#include "Snake.hpp"
#include "Collision.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <time.h>

//za prikaz
SDL_Renderer *renderer;
bool running = true;
int dir = 0;

//za score
TTF_Font *font;
int score = 0;

//objekti
Snake snake;
Food apple;

const int WIDTH = 1000;
const int HEIGHT = 1000;

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum GameState
{
    PLAY,
    PAUSED
};


GameState state = PLAY;

void renderTextScore() {
    if(font == NULL) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, ("Score: " + std::to_string(score)).c_str(), textColor);
    if (textSurface == NULL) {
        std::cout << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void main_loop()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
            running = false;
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_DOWN && dir != UP)
            {
                dir = DOWN;
            }
            if (e.key.keysym.sym == SDLK_UP && dir != DOWN)
            {
                dir = UP;
            }
            if (e.key.keysym.sym == SDLK_LEFT && dir != RIGHT)
            {
                dir = LEFT;
            }
            if (e.key.keysym.sym == SDLK_RIGHT && dir != LEFT)
            {
                dir = RIGHT;
            }
            if (e.key.keysym.sym == SDLK_SPACE)
            {
                
                if (state == PLAY)
                {
                    state = PAUSED;
                }
                else
                {
                    state = PLAY;
                }
            }
        }
    }

    //pomakni zmiju
    if (state == PLAY) {
        snake.move(dir);
    }

    //provjera kolizija
    if (Collision::selfCollision(snake))
    {
        snake.reset();
        score = 0;
    }

    if(Collision::foodCollision(snake.getHead(), apple.getFood())) {
        snake.grow();
        SDL_Rect newFoodPosition = {-10, -10, 10, 10};
        apple.setFood(newFoodPosition);
        apple.generateFood(WIDTH, HEIGHT);
        score++;
    }
    
    if (Collision::wallCollision(snake.getHead(), WIDTH, HEIGHT))
    {
        snake.reset();
        score = 0;
    }

    //crtanje na ekran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //nacrtaj glavu zmije
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &snake.getHead());

    //nacrtaj tijelo zmije
    for (auto& snake_part : snake.getBody())
    {
        SDL_RenderFillRect(renderer, &snake_part);
    }

    //nacrtaj jabuku
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &apple.getFood());

    //prikaz score-a
    renderTextScore();

    //prikaz svega
    SDL_RenderPresent(renderer);
    SDL_Delay(25);
}

//probavanje timer funkcije
Uint32 timerCallback(Uint32 interval, void *param)
{
    std::cout << "Timer istekao!" << std::endl;
    return interval;
}

int main(int argc, char *argv[])
{
    //inicijalizacija SDL-a
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (TTF_Init() != 0) {
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //font
    font = TTF_OpenFont("Jersey10-Regular.ttf", 24);
    if (font == NULL) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Window *win = SDL_CreateWindow("Snake_Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (win == NULL) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(win, 1, 0);
        if (renderer == NULL) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //postavljanje timer funkcije (proba)
    SDL_TimerID timerID = SDL_AddTimer(5000, timerCallback, NULL);

    //postavljanje seed-a za random
    srand(time(0));

    //postavljanje jabuke
    apple.generateFood(WIDTH, HEIGHT);

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, 1);
    #else
        while (running)
        {
            main_loop();
        }
    #endif
    
    //ciscenje
    SDL_RemoveTimer(timerID);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}