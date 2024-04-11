#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <random>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <time.h>

SDL_Renderer *rendere;
bool running = true;
int dir = 0;
//glava zmije
SDL_Rect head = {500, 500, 10, 10};
//Snake tijelo
std::deque<SDL_Rect> snake;
//velicina zmije
int snake_size = 1;
//jabuka
SDL_Rect apple;

const int WIDTH = 1000;
const int HEIGHT = 1000;

//brzina zmije
int snake_speed = 5;

//jabuke
std::vector<SDL_Rect> apples;

void make_apples() {
    //generiranje jabuke
        apple.x = rand()%100*10;
        apple.y = rand()%100*10;
        apple.w = 10;
        apple.h = 10;
}

void reset_game() {
    snake_speed = 5;
    snake.clear();
    snake_size = 1;
    head.x = 500;
    head.y = 500;
}

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

    //komande za zmiju
    if (state == PLAY) {
            switch (dir)
        {
            case DOWN:
                head.y += snake_speed;
                break;
            case UP:
                head.y -= snake_speed;
                break;
            case LEFT:
                head.x -= snake_speed;
                break;
            case RIGHT:
                head.x += snake_speed;
                break;
        }
            //kolizija zmije sa samom sobom
        std::for_each(snake.begin(), snake.end(), [&](auto& snake_part){
            if(head.x == snake_part.x && head.y == snake_part.y) {
                snake_size = 1;
                snake_speed = 5;
            }
        });
    }


    //kolizija zmije s jabukom
    if((head.x - apple.x)*(head.x - apple.x) + (head.y - apple.y)*(head.y - apple.y) < 100) {
        snake_speed+=1;
        snake_size+=10;
        apple.x = -10;
        apple.y = -10;
        make_apples();
    }

    

    //koiizija zmije sa zidom
    if (head.x < 0 || head.x > WIDTH || head.y < 0 || head.y > HEIGHT) {
        reset_game();
    }

    //dodavanje nove glave zmije
    snake.push_front(head);

    //ukoliko je zmija prevelika, ukloni zadnji dio
    while(snake.size() > snake_size) {
        snake.pop_back();
    }

    // obrisi ekran
    SDL_SetRenderDrawColor(rendere, 0, 0, 0, 255);
    SDL_RenderClear(rendere);

    // nacrtaj zmiju
    //glava zmije
    SDL_SetRenderDrawColor(rendere, 255, 255, 255, 255);
    //SDL_RenderFillRect(rendere, &head);

    //tijelo zmije
    std::for_each(snake.begin(), snake.end(), [&](SDL_Rect &snake_part){
        SDL_RenderFillRect(rendere, &snake_part); //nactaj tijelo zmije
    });

    //nacrtaj jabuke
    SDL_SetRenderDrawColor(rendere, 255, 0, 0, 255);
    SDL_RenderFillRect(rendere, &apple);

    //prikaz
    SDL_RenderPresent(rendere);
    SDL_Delay(25);
}

Uint32 timerCallback(Uint32 interval, void *param)
{
    std::cout << "Timer istekao!" << std::endl;
    //make_apples();
    return interval;
}

int main(int argc, char *argv[])
{

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

    SDL_TimerID timerID = SDL_AddTimer(5000, timerCallback, NULL);

    srand(time(0));
    make_apples();

    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, 1);
    #else
        while (running)
        {
            main_loop();
        }
    #endif
    std::cout << "Izlaz iz petlje" << std::endl;
    SDL_RemoveTimer(timerID);
    SDL_DestroyRenderer(rendere);
    std::cout << "Izlaz iz petlje" << std::endl;
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}