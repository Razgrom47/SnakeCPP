#include <iostream>
#include "Game.h"
#include <cstdlib> 
#include <ctime>

Game::Game() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    window = SDL_CreateWindow("Snake Game", 
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              0);
    if (!window) {
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        return;
    }
    
    // Optional: Set the window title explicitly
    SDL_SetWindowTitle(window, "Snake Game");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation error: " << SDL_GetError() << std::endl;
        return;
    }

    ui = new UI(renderer);
    if (!ui) {
        std::cerr << "UI creation failed!" << std::endl;
        return;
    }

    srand(static_cast<unsigned int>(time(NULL)));
    reset();
    initialized = true;
}

Game::~Game() {
    delete ui;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::reset() {
    snake.reset();
    gameOver = false;
    startTime = SDL_GetTicks();
    spawnFood();
}

void Game::spawnFood() {
    do {
        food.x = rand() % (SCREEN_WIDTH / GRID_SIZE);
        food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE);
    } while (snake.checkSelfCollision(food));
}

void Game::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        if (gameOver && ui->handleRestartButton(e)) reset();
        if (e.type == SDL_KEYDOWN) snake.changeDirection(e.key.keysym.sym);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
    SDL_RenderClear(renderer);

    ui->renderGrid();

    // Draw snake
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    for (const auto& segment : snake.body) {
        SDL_Rect block = { segment.x * GRID_SIZE, segment.y * GRID_SIZE, 
                           GRID_SIZE - 1, GRID_SIZE - 1 };
        SDL_RenderFillRect(renderer, &block);
    }

    // Draw food
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_Rect foodRect = { food.x * GRID_SIZE, food.y * GRID_SIZE,
                          GRID_SIZE - 1, GRID_SIZE - 1 };
    SDL_RenderFillRect(renderer, &foodRect);

    ui->renderScore(snake.score, SDL_GetTicks() - startTime);
    if (gameOver) ui->renderGameOver(snake.score, SDL_GetTicks() - startTime);

    SDL_RenderPresent(renderer);
}

void Game::run() {
    running = true;
    Uint32 lastUpdate = SDL_GetTicks();
    
    while (running) {
        handleInput();
        
        Uint32 currentTime = SDL_GetTicks();
        if (!gameOver && currentTime - lastUpdate >= FRAME_DELAY) {
            snake.update(gameOver);
            
            // Check food collision
            if (snake.body.front().x == food.x && 
                snake.body.front().y == food.y) {
                snake.score++;
                snake.shouldGrow = true;  // This makes the snake grow
                spawnFood();
            }
            
            lastUpdate = currentTime;
        }
        
        render();
        SDL_Delay(16);
    }
}
