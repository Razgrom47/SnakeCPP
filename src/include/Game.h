#pragma once
#include "Snake.h"
#include "UI.h"
#include "Constants.h"

class Game {
public:
    Game();
    ~Game();
     
    void run();
    bool isInitialized() const { return initialized; }

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Snake snake;
    UI* ui = nullptr;
    SDL_Point food;
    bool running = false;
    bool gameOver = false;
    bool initialized = false;
    Uint32 startTime = 0;
    void reset(); 
    void spawnFood();
    void handleInput();
    void render();
};
