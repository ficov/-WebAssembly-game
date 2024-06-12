#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Food.hpp"
#include "Snake.hpp"
#include "Collision.hpp"
#include "Walls.hpp"
#include "GameElements.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <time.h>

//konstante
const int WIDTH = 950;
const int HEIGHT = 900;

GameElements gameElements; //struktura za elemente igre

//funkcija za prikaz teksta
void renderText(const std::string &text, int x, int y, const SDL_Color &textColor) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(gameElements.font, text.c_str(), textColor);
    if (textSurface == NULL) {
        std::cout << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(gameElements.renderer, textSurface);
    if (textTexture == NULL) {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(gameElements.renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void resetGameStats() {
    gameElements.score = 0;
    gameElements.lives = 3;
    gameElements.level = 1;
}

void clear_game_elements() {
    SDL_DestroyTexture(gameElements.speedTexture);
    SDL_DestroyTexture(gameElements.scoreTexture);
    SDL_DestroyTexture(gameElements.levelTexture);
    SDL_DestroyTexture(gameElements.livesTexture);
    SDL_DestroyTexture(gameElements.spriteSheetTexture);
    SDL_DestroyTexture(gameElements.iconTexture);
    SDL_DestroyRenderer(gameElements.renderer);
    SDL_DestroyWindow(gameElements.win);
    TTF_CloseFont(gameElements.font);
    TTF_Quit();
    SDL_Quit();
}

void handleKeyPress(SDL_Event &event) {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
        gameElements.running = false;
    }
    if (gameElements.state == MENU || gameElements.state == WINNER || gameElements.state == DEAD || gameElements.state == MINUS_LIFE || gameElements.state == LEVEL_UP) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            gameElements.state = PLAY;
        }
        return;
    }
    if(gameElements.state == PLAY) {
        if (event.key.keysym.sym == SDLK_DOWN && gameElements.dir != UP)
        {
            gameElements.dir = DOWN;
        }
        if (event.key.keysym.sym == SDLK_UP && gameElements.dir != DOWN)
        {
            gameElements.dir = UP;
        }
        if (event.key.keysym.sym == SDLK_LEFT && gameElements.dir != RIGHT)
        {
            gameElements.dir = LEFT;
        }
        if (event.key.keysym.sym == SDLK_RIGHT && gameElements.dir != LEFT)
        {
            gameElements.dir = RIGHT;
        }
    }
    if (gameElements.state == PAUSED || gameElements.state == PLAY) {
        if (event.key.keysym.sym == SDLK_SPACE)
        {
            if (gameElements.state == PAUSED)
            {
                gameElements.state = PLAY;
            }
            else
            {
                gameElements.state = PAUSED;
            }
        }
        if (event.key.keysym.sym == SDLK_RETURN && gameElements.state == PAUSED)
        {
            gameElements.snake.reset(true);
            resetGameStats();
            gameElements.state = PLAY;
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
            gameElements.running = false;
        }
        if (e.type == SDL_KEYDOWN)
        {
            handleKeyPress(e);
        }
    }

    bool resetSpeed = true;

    //crtanje na ekran
    SDL_SetRenderDrawColor(gameElements.renderer, 0, 50, 0, 255);
    SDL_RenderClear(gameElements.renderer);

    //nacrtaj tijelo zmije
    gameElements.snake.renderBody(gameElements.renderer, gameElements.spriteSheetTexture, gameElements.dir);

    //nacrtaj glavu zmije
    gameElements.snake.renderHead(gameElements.renderer, gameElements.spriteSheetTexture, gameElements.dir);
    
    //nacrtaj zidove
    gameElements.walls.renderWalls(gameElements.renderer);

    //prikaz jabuke
    gameElements.apple.renderFood(gameElements.renderer, gameElements.iconTexture);

    //provjera stanja igre
    switch (gameElements.state) {
        case MENU:
            renderText("Press Enter to start", WIDTH / 2 - 180, HEIGHT / 2, {255, 255, 255});
            break;
        case PAUSED:          
            renderText("Paused", WIDTH / 2 - 68, HEIGHT / 2 - 20, {255, 255, 255});
            renderText("Press SPACE to return", WIDTH / 2 - 188, HEIGHT / 2 + 20, {255, 255, 255});
            renderText("Press ENTER to restart", WIDTH / 2 - 188, HEIGHT / 2 + 60, {255, 255, 255});
            break;
        case WINNER:
            renderText("You won!", WIDTH / 2 - 68, HEIGHT / 2 - 20, {255, 255, 255});
            renderText("Your total score: " + std::to_string(gameElements.totalScore), WIDTH / 2 - 160, HEIGHT / 2 + 20, {255, 255, 255});
            renderText("Press Enter to start again", WIDTH / 2 - 188, HEIGHT / 2 + 60, {255, 255, 255});
            break;
        case DEAD:
            renderText("You lost!", WIDTH / 2 - 68, HEIGHT / 2 - 20, {255, 255, 255});
            renderText("Your total score: " + std::to_string(gameElements.totalScore), WIDTH / 2 - 160, HEIGHT / 2 + 20, {255, 255, 255});
            renderText("Press Enter to start again", WIDTH / 2 - 190, HEIGHT / 2 + 60, {255, 255, 255});
            break;
        case MINUS_LIFE:
            renderText("You lost a life!", WIDTH / 2 - 130, HEIGHT / 2 - 10, {255, 255, 255});
            renderText("Press Enter to continue", WIDTH / 2 - 175, HEIGHT / 2 + 30, {255, 255, 255});
            break;
        case LEVEL_UP:
            renderText("Level up!", WIDTH / 2 - 68, HEIGHT / 2 - 20, {255, 255, 255});
            renderText("Speed is increased", WIDTH / 2 - 160, HEIGHT / 2 + 20, {255, 255, 255});
            renderText("Press Enter to continue", WIDTH / 2 - 175, HEIGHT / 2 + 60, {255, 255, 255});
            break;
        case PLAY:
            //pomakni zmiju
            gameElements.snake.move(gameElements.dir);
            //provjera kolizija
            if (Collision::selfCollision(gameElements.snake))
            {
                --gameElements.lives;
                resetSpeed = false;
                gameElements.snake.reset(resetSpeed);
                resetSpeed = true;
                if (gameElements.lives == 0) {
                    gameElements.snake.reset(resetSpeed);
                    gameElements.state = DEAD;
                    gameElements.totalScore = gameElements.score;
                    resetGameStats();
                } else {
                    gameElements.state = MINUS_LIFE;
                }
            }

            else if(Collision::foodCollision(gameElements.snake.getHead(), gameElements.apple.getFood())) {
                gameElements.snake.grow();
                SDL_Rect newFoodPosition = {-10, -10, 40, 40};
                gameElements.apple.setFood(newFoodPosition);
                gameElements.apple.generateFood(WIDTH, HEIGHT, gameElements.snake.getHead(), gameElements.walls.getTopWall(), gameElements.walls.getBottomWall(), gameElements.walls.getLeftWall(), gameElements.walls.getRightWall());
                gameElements.score++;
            }
            
            else if (Collision::wallCollision(gameElements.snake.getHead(), gameElements.walls.getTopWall(), gameElements.walls.getBottomWall(), gameElements.walls.getLeftWall(), gameElements.walls.getRightWall()))
            {
                --gameElements.lives;
                resetSpeed = false;
                gameElements.snake.reset(resetSpeed);
                resetSpeed = true;
                if (gameElements.lives == 0) {
                    gameElements.snake.reset(resetSpeed);
                    gameElements.state = DEAD;
                    gameElements.totalScore = gameElements.score;
                    resetGameStats();
                } else {
                    gameElements.state = MINUS_LIFE;
                }
            }
            
            if (gameElements.score == 2 * gameElements.level) {
                if (gameElements.score == 10) {
                    gameElements.snake.reset(resetSpeed);
                    gameElements.state = WINNER;
                    gameElements.totalScore = gameElements.score;
                    resetGameStats();
                    break;
                } else {
                    ++gameElements.level;
                    gameElements.state = LEVEL_UP;
                    gameElements.snake.increaseSpeed();
                }
            }
    }

    //prikaz score-a
    SDL_RenderCopy(gameElements.renderer, gameElements.scoreTexture, NULL, &gameElements.scoreRect);
    renderText(" : " + std::to_string(gameElements.score), 140, 10, {255, 255, 255});

    //prikaz zivota
    SDL_RenderCopy(gameElements.renderer, gameElements.livesTexture, NULL, &gameElements.livesRect);
    renderText(" : " + std::to_string(gameElements.lives), 340, 10, {255, 255, 255});

    //prikaz levela
    SDL_RenderCopy(gameElements.renderer, gameElements.levelTexture, NULL, &gameElements.levelRect);
    renderText(" : " + std::to_string(gameElements.level), 540, 10, {255, 255, 255});

    //prikaz brzine
    SDL_RenderCopy(gameElements.renderer, gameElements.speedTexture, NULL, &gameElements.speedRect);
    renderText(" : " + std::to_string(gameElements.snake.getSpeed()-5-(gameElements.level-1)*2), 730, 10, {255, 255, 255});

    //prikaz svega
    SDL_RenderPresent(gameElements.renderer);
    SDL_Delay(25);
}

int main(int argc, char *argv[])
{
    gameElements.state = MENU;
    gameElements.apple.generateFood(WIDTH, HEIGHT, gameElements.snake.getHead(), gameElements.walls.getTopWall(), gameElements.walls.getBottomWall(), gameElements.walls.getLeftWall(), gameElements.walls.getRightWall());
    gameElements.walls.generateWalls(WIDTH, HEIGHT);

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

    gameElements.font = TTF_OpenFont("Jersey10-Regular.ttf", 50);
    if (gameElements.font == NULL) {
        std::cout << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //inicijalizacija SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cout << "IMG_Init Error: " << IMG_GetError() << std::endl;
        TTF_CloseFont(gameElements.font);
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    gameElements.win = SDL_CreateWindow("Snake_Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (gameElements.win == NULL) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    gameElements.renderer = SDL_CreateRenderer(gameElements.win, 1, 0);
        if (gameElements.renderer == NULL) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(gameElements.win);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    //postavljanje ikone
    SDL_Surface *icon = IMG_Load("apple.png");

    gameElements.iconTexture = SDL_CreateTextureFromSurface(gameElements.renderer, icon);

    SDL_FreeSurface(icon);

    SDL_Surface *spriteSheetSurface = IMG_Load("snake-sprite.png");

    gameElements.spriteSheetTexture = SDL_CreateTextureFromSurface(gameElements.renderer, spriteSheetSurface);

    SDL_FreeSurface(spriteSheetSurface);

    SDL_Surface *livesSurface = IMG_Load("heart.png");

    gameElements.livesTexture = SDL_CreateTextureFromSurface(gameElements.renderer, livesSurface);

    SDL_FreeSurface(livesSurface);

    SDL_Surface *levelSurface = IMG_Load("level.png");

    gameElements.levelTexture = SDL_CreateTextureFromSurface(gameElements.renderer, levelSurface);

    SDL_FreeSurface(levelSurface);

    SDL_Surface *scoreSurface = IMG_Load("score.png");

    gameElements.scoreTexture = SDL_CreateTextureFromSurface(gameElements.renderer, scoreSurface);

    SDL_FreeSurface(scoreSurface);

    SDL_Surface *speedSurface = IMG_Load("speed.png");

    gameElements.speedTexture = SDL_CreateTextureFromSurface(gameElements.renderer, speedSurface);

    SDL_FreeSurface(speedSurface);

    if (icon == NULL || spriteSheetSurface == NULL || livesSurface == NULL || levelSurface == NULL || scoreSurface == NULL || speedSurface == NULL
        || gameElements.iconTexture == NULL || gameElements.spriteSheetTexture == NULL || gameElements.livesTexture == NULL || gameElements.levelTexture == NULL 
        || gameElements.scoreTexture == NULL || gameElements.speedTexture == NULL) {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        clear_game_elements();
        return EXIT_FAILURE;
    }

    //postavljanje seed-a za random
    srand(time(0));

    //glavna petlja
    #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, 1);
    #else
        while (gameElements.running)
        {
            main_loop();
        }
    #endif
    
    clear_game_elements();

    return EXIT_SUCCESS;
}