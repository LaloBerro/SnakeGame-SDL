#include "SDL.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include "SDL_image.h"

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
int snakeLength = 0;
int snakeDirection = 0;
int snakeSpeed = 10; //10
int snakeWidth = 30; //10
int snakeHeight = 30;
int amountOfSnakeBodyIncrease = 10;

int minDistanceToCollideWithApple = 20;

SDL_Rect appleRect;

bool isApplicationRunning = true;

int snakeHeadTextureWidth = 32;
int snakeHeadTextureHeight = 32;
SDL_Texture *snakeHeadTexture;
SDL_Rect snakeHeadSourceRect;
SDL_Rect snakeHeadDestinationRect;

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
void DrawSnakeHead();
void DrawSnakeSegment(SDL_Rect snakeSegmentRect);
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
    freopen("CON", "w", stdout); // redirects stdout
    freopen("CON", "w", stderr); // redirects stderr

    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    snakeHeadRect = {windowWidth/2, windowHeight/2, snakeWidth, snakeHeight};
    appleRect = {rand()%100*10, rand()%100*10, 10, 10};

    int flags = IMG_INIT_PNG;
    int initStatus = IMG_Init(flags);
    if((initStatus & flags) != flags)
    {
        std::cout << "SDL2_Image PNG Format not available" << std::endl;
    }
    
    SDL_Surface* tmpSurface = IMG_Load("../assets/snake.png");
    if (!tmpSurface)
    {
        std::cout << "SDL2_Image image not found" << std::endl;
    }
    
    
    snakeHeadTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

    SDL_FreeSurface(tmpSurface);
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
    int xDistance = snakeHeadRect.x - appleRect.x;
    if (xDistance < 0)
        xDistance *= -1;

    int yDistance = snakeHeadRect.y - appleRect.y;
    if (yDistance < 0)
        yDistance *= -1;

    if (xDistance >= minDistanceToCollideWithApple || yDistance > minDistanceToCollideWithApple)
        return;
    
    snakeLength += amountOfSnakeBodyIncrease;
    appleRect.x = rand()%100*10;
    appleRect.y = rand()%100*10;
}

void SnakeBodyCollisionDetection()
{
    if (snakeBodyRectDeque.size() <= 1)
        return;
    
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

    if (snakeHeadRect.x > windowWidth)
        snakeHeadRect.x -= windowWidth;
    
    else if (snakeHeadRect.x < 0)
        snakeHeadRect.x += windowWidth;

    else if (snakeHeadRect.y > windowHeight)
        snakeHeadRect.y -= windowHeight;

    else if (snakeHeadRect.y < 0)
        snakeHeadRect.y += windowHeight;
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
    
    for (int i = snakeLength; i >= 0 ; --i)
    {
        if (i <= 0)
            DrawSnakeHead();
        
        else
            DrawSnakeSegment(snakeBodyRectDeque[i]);
    }
}

void DrawSnakeHead()
{
    SDL_Rect crop = {0, 0, snakeHeadTextureWidth, snakeHeadTextureHeight};
    float angle = 0;

    switch (snakeDirection)
    {
        case DOWN:
            angle = 180.0f;
            break;

        case UP:
            angle = 0.0f;
            break;

        case LEFT:
            angle = -90.0f;
            break;

        case RIGHT:
            angle = 90.0f;
            break;
    }

    SDL_Point center = {snakeWidth / 2, snakeHeight / 2};
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    SDL_RenderCopyEx(renderer, snakeHeadTexture, &crop , &snakeHeadRect, angle, &center, flip);
}

void DrawSnakeSegment(SDL_Rect snakeSegmentRect)
{
    snakeSegmentRect.w = 28;
    SDL_RenderFillRect(renderer, &snakeSegmentRect);
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