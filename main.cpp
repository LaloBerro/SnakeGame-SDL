#include "SDL.h"
#include <algorithm>
#include <deque>

enum Direction
{
    DOWN,
    LEFT,
    RIGHT,
    UP
};

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Event event;

int windowWidth = 1000;
int windowHeight = 1000;

SDL_Rect snakeHeadRect;
std::deque<SDL_Rect> snakeBodyRectDeque;
int snakeLength = 1;
int snakeDirection = 0;
int snakeSpeed = 10;
int snakeWidth = 10;
int snakeHeight = 10;
int amountOfSnakeBodyIncrease = 10;

SDL_Rect appleRect;

bool isApplicationRunning = true;

void Initialize();
void ExecuteGameLoop();
void CheckInputs();
void Move();
void AppleCollisionDetection();
void SnakeBodyCollisionDetection();
void WallCollisionDetection();
void Die();
void SetSnakeSize();
void ClearWindow();
void DrawBody();
void DrawApples();
void Display();

int main(int argc, char *argv[])
{
    Initialize();

    ExecuteGameLoop();

    return 0;
}

void Initialize()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    snakeHeadRect = {windowWidth/2, windowHeight/2, snakeWidth, snakeHeight};
    appleRect = {rand()%100*10, rand()%100*10, 10, 10};
}

void ExecuteGameLoop()
{
    while (isApplicationRunning)
    {
        CheckInputs();

        Move();

        AppleCollisionDetection();
        SnakeBodyCollisionDetection();
        WallCollisionDetection();

        SetSnakeSize();

        ClearWindow();

        DrawBody();
        DrawApples();

        Display();
    }
}

void CheckInputs()
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            isApplicationRunning = false;

        if (event.type != SDL_KEYDOWN)
            continue;

        if (event.key.keysym.sym == SDLK_DOWN)
            snakeDirection = DOWN;

        if (event.key.keysym.sym == SDLK_UP)
            snakeDirection = UP;

        if (event.key.keysym.sym == SDLK_RIGHT)
            snakeDirection = RIGHT;

        if (event.key.keysym.sym == SDLK_LEFT)
            snakeDirection = LEFT;
    }
}

void Move()
{
    switch (snakeDirection)
    {
        case DOWN:
            snakeHeadRect.y += snakeSpeed;
            break;

        case UP:
            snakeHeadRect.y -= snakeSpeed;
            break;

        case LEFT:
            snakeHeadRect.x -= snakeSpeed;
            break;

        case RIGHT:
            snakeHeadRect.x += snakeSpeed;
            break;
    }
}

void AppleCollisionDetection()
{
    if (snakeHeadRect.x != appleRect.x || snakeHeadRect.y != appleRect.y)
        return;

    snakeLength += amountOfSnakeBodyIncrease;
    appleRect.x = rand()%100*10;
    appleRect.y = rand()%100*10;
}

void SnakeBodyCollisionDetection()
{
    std::for_each(snakeBodyRectDeque.begin(), snakeBodyRectDeque.end(), [&](auto & snakeSegment)
    {
        if (snakeHeadRect.x == snakeSegment.x && snakeHeadRect.y == snakeSegment.y)
            Die();
    });
}

void WallCollisionDetection()
{
    if (snakeHeadRect.x <= windowWidth && snakeHeadRect.y <= windowHeight && snakeHeadRect.x >= 0 && snakeHeadRect.y >= 0)
        return;

    Die();
}

void Die()
{
    snakeHeadRect.x = windowWidth/2;
    snakeHeadRect.y = windowHeight/2;

    snakeLength = 1;
}

void SetSnakeSize()
{
    snakeBodyRectDeque.push_front(snakeHeadRect);

    while (snakeBodyRectDeque.size() > snakeLength)
    {
        snakeBodyRectDeque.pop_back();
    }
}

void ClearWindow()
{
    SDL_SetRenderDrawColor(renderer, 0 ,0 ,0, 255);
    SDL_RenderClear(renderer);
}

void DrawBody()
{
    SDL_SetRenderDrawColor(renderer, 52, 235, 82, 255);

    std::for_each(snakeBodyRectDeque.begin(), snakeBodyRectDeque.end(), [&](auto& snakeSegment)
    {
        SDL_RenderFillRect(renderer, &snakeSegment);
    });
}

void DrawApples()
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &appleRect);
}

void Display()
{
    SDL_RenderPresent(renderer);
    SDL_Delay(25);
}