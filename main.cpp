#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Food.hpp"
#include "Snake.hpp"
#include "Collision.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <time.h>

//konstante
const int WIDTH = 950;
const int HEIGHT = 900;

//za prikaz
SDL_Renderer *renderer;
SDL_Texture *iconTexture;
bool running = true;
int dir = 0;

//zivoti
int lives = 3;

//level
int level = 1;

//za score
TTF_Font *font;
int score = 0;
int totalScore = 0;

//objekti
Snake snake;
Food apple;

//zidovi (x, y, w, h), pravokutnici
SDL_Rect topWall = {0, 0, WIDTH, 20};
SDL_Rect bottomWall = {0, HEIGHT - 20, WIDTH, 20};
SDL_Rect leftWall = {0, 0, 20, HEIGHT};
SDL_Rect rightWall = {WIDTH - 20, 0, 20, HEIGHT};

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
    PAUSED,
    MENU,
    WINNER,
    DEAD,
    MINUS_LIFE,
    LEVEL_UP
};


GameState state = MENU;

//funkcija za prikaz teksta
void renderText(const std::string &text, int x, int y, const SDL_Color &textColor) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface == NULL) {
        std::cout << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void handleKeyPress(SDL_Event &event) {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
        running = false;
    }
    if (state == MENU || state == WINNER || state == DEAD || state == MINUS_LIFE || state == LEVEL_UP) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            state = PLAY;
        }
        return;
    }
    if (event.key.keysym.sym == SDLK_DOWN && dir != UP)
    {
        dir = DOWN;
    }
    if (event.key.keysym.sym == SDLK_UP && dir != DOWN)
    {
        dir = UP;
    }
    if (event.key.keysym.sym == SDLK_LEFT && dir != RIGHT)
    {
        dir = LEFT;
    }
    if (event.key.keysym.sym == SDLK_RIGHT && dir != LEFT)
    {
        dir = RIGHT;
    }
    if (state == PAUSED || state == PLAY) {
        if (event.key.keysym.sym == SDLK_SPACE)
        {
            if (state == PAUSED)
            {
                state = PLAY;
            }
            else
            {
                state = PAUSED;
            }
        }
        if (event.key.keysym.sym == SDLK_RETURN && state == PAUSED)
        {
            snake.reset();
            score = 0;
            lives = 3;
            level = 1;
            state = PLAY;
        }
    }
}

//glavna petlja
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
            handleKeyPress(e);
        }
    }

    //crtanje na ekran
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);
    SDL_RenderClear(renderer);

    //nacrtaj glavu zmije
    SDL_SetRenderDrawColor(renderer, 0, 130, 0, 255);
    SDL_RenderFillRect(renderer, &snake.getHead());

    //nacrtaj zidove
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_RenderFillRect(renderer, &topWall);
    SDL_RenderFillRect(renderer, &bottomWall);
    SDL_RenderFillRect(renderer, &leftWall);
    SDL_RenderFillRect(renderer, &rightWall);

    //nacrtaj tijelo zmije
    for (auto& snake_part : snake.getBody())
    {
        SDL_RenderFillRect(renderer, &snake_part);
    }

    //prikaz jabuke
    SDL_RenderCopy(renderer, iconTexture, NULL, &apple.getFood());

    //provjera stanja igre
    switch (state) {
        case MENU:
            renderText("Press Enter to start", WIDTH / 2 - 180, HEIGHT / 2, {255, 255, 255});
            break;
        case PAUSED:          
            renderText("Paused", WIDTH / 2 - 50, HEIGHT / 2 - 20, {255, 255, 255});
            renderText("Press SPACE to return", WIDTH / 2 - 170, HEIGHT / 2 + 20, {255, 255, 255});
            renderText("Press ENTER to restart", WIDTH / 2 - 170, HEIGHT / 2 + 60, {255, 255, 255});
            break;
        case WINNER:
            renderText("You won!", WIDTH / 2 - 60, HEIGHT / 2 - 20, {255, 255, 255});
            renderText("Your total score: " + std::to_string(totalScore), WIDTH / 2 - 160, HEIGHT / 2 + 20, {255, 255, 255});
            renderText("Press Enter to start again", WIDTH / 2 - 190, HEIGHT / 2 + 60, {255, 255, 255});
            break;
        case DEAD:
            renderText("You lost!", WIDTH / 2 - 60, HEIGHT / 2 - 20, {255, 255, 255});
            renderText("Your total score: " + std::to_string(totalScore), WIDTH / 2 - 160, HEIGHT / 2 + 20, {255, 255, 255});
            renderText("Press Enter to start again", WIDTH / 2 - 190, HEIGHT / 2 + 60, {255, 255, 255});
            break;
        case MINUS_LIFE:
            renderText("You lost a life!", WIDTH / 2 - 130, HEIGHT / 2 - 10, {255, 255, 255});
            renderText("Press Enter to continue", WIDTH / 2 - 175, HEIGHT / 2 + 30, {255, 255, 255});
            break;
        case LEVEL_UP:
            renderText("Level up!", WIDTH / 2 - 60, HEIGHT / 2 - 20, {255, 255, 255});
            renderText("Speed is increased", WIDTH / 2 - 160, HEIGHT / 2 + 20, {255, 255, 255});
            renderText("Press Enter to continue", WIDTH / 2 - 175, HEIGHT / 2 + 60, {255, 255, 255});
            break;
        case PLAY:
            //pomakni zmiju
            snake.move(dir);
            //provjera kolizija
            if (Collision::selfCollision(snake))
            {
                if (level > 1) {
                    level = 1;
                }
                --lives;
                snake.reset();
                score = 0;
                if (lives == 0) {
                    state = DEAD;
                    totalScore = score;
                    score = 0;
                    lives = 3;
                    level = 1;
                } else {
                    state = MINUS_LIFE;
                }
            }

            if(Collision::foodCollision(snake.getHead(), apple.getFood())) {
                snake.grow();
                SDL_Rect newFoodPosition = {-10, -10, 40, 40};
                apple.setFood(newFoodPosition);
                apple.generateFood(WIDTH, HEIGHT);
                score++;
            }
            
            if (Collision::wallCollision(snake.getHead(), topWall, bottomWall, leftWall, rightWall))
            {
                --lives;
                if (level > 1) {
                    level = 1;
                }
                score = 0;
                snake.reset();
                if (lives == 0) {
                    state = DEAD;
                    totalScore = score;
                    lives = 3;
                    score = 0;
                    level = 1;
                } else {
                    state = MINUS_LIFE;
                }
            }
            if (score == 1 * level) {
                if (score == 5) {
                    snake.reset();
                    state = WINNER;
                    totalScore = score;
                    score = 0;
                    lives = 3;
                    level = 1;
                    break;
                } else {
                    ++level;
                    state = LEVEL_UP;
                    snake.increaseSpeed();
                }
            }
    }

    //prikaz score-a
    renderText("Score: " + std::to_string(score), 100, 10, {255, 255, 255});

    //prikaz zivota
    renderText("Lives: " + std::to_string(lives), 300, 10, {255, 255, 255});

    //prikaz levela
    renderText("Level: " + std::to_string(level), 500, 10, {255, 255, 255});

    //prikaz brzine
    renderText("Speed: " + std::to_string(snake.getSpeed()), 700, 10, {255, 255, 255});

    //prikaz svega
    SDL_RenderPresent(renderer);
    SDL_Delay(25);
}

//probavanje timer funkcije
Uint32 timerCallback(Uint32 interval, void *param)
{
    //std::cout << "Timer istekao!" << std::endl;
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
    font = TTF_OpenFont("Jersey10-Regular.ttf", 50);
    if (font == NULL) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //inicijalizacija SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cout << "IMG_Init Error: " << IMG_GetError() << std::endl;
        TTF_CloseFont(font);
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

    //postavljanje ikone
    SDL_Surface *icon = IMG_Load("apple.png");
    if (icon == NULL) {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    iconTexture = SDL_CreateTextureFromSurface(renderer, icon);
    if (iconTexture == NULL) {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(icon);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_FreeSurface(icon);

    //postavljanje timer funkcije (proba)
    SDL_TimerID timerID = SDL_AddTimer(5000, timerCallback, NULL);

    //postavljanje seed-a za random
    srand(time(0));

    //postavljanje jabuke
    apple.generateFood(WIDTH, HEIGHT);

    //glavna petlja
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
    SDL_DestroyTexture(iconTexture);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}