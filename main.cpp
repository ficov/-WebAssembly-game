#include <iostream>
#include <SDL2/SDL.h>
#include "Food.hpp"
#include "Snake.hpp"
#include "Collision.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <time.h>

//za prikaz
SDL_Renderer *rendere;
bool running = true;
int dir = 0;

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
    }

    if(Collision::foodCollision(snake.getHead(), apple.getFood())) {
        snake.grow();
        SDL_Rect newFoodPosition = {-10, -10, 10, 10};
        apple.setFood(newFoodPosition);
        apple.generateFood(WIDTH, HEIGHT);
    }
    
    if (Collision::wallCollision(snake.getHead(), WIDTH, HEIGHT))
    {
        snake.reset();
    }

    //crtanje na ekran
    SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
    SDL_RenderClear(rendere);

    //nacrtaj glavu zmije
    SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
    SDL_RenderFillRect(rendere, &snake.getHead());

    //nacrtaj tijelo zmije
    for (auto& snake_part : snake.getBody())
    {
        SDL_RenderFillRect(rendere, &snake_part);
    }

    //nacrtaj jabuku
    SDL_SetRenderDrawColor(rendere, 255, 0, 0, 255);
    SDL_RenderFillRect(rendere, &apple.getFood());

    //prikaz svega
    SDL_RenderPresent(rendere);
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

    SDL_Window *win = SDL_CreateWindow("Snake_Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (win == NULL) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    rendere = SDL_CreateRenderer(win, 1, 0);
        if (rendere == NULL) {
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
    std::cout << "Izlaz iz petlje" << std::endl;
    SDL_RemoveTimer(timerID);
    SDL_DestroyRenderer(rendere);
    std::cout << "Izlaz iz petlje" << std::endl;
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}