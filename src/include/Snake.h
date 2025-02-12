#pragma once
#include <SDL2/SDL.h>
#include <vector>

struct Snake {
    std::vector<SDL_Point> body;
    SDL_Point direction = {1, 0};
    int score = 0;
    bool shouldGrow = false;
    
    void reset(); 
    void update(bool& gameOver);
    void changeDirection(SDL_Keycode key);
    bool checkSelfCollision(const SDL_Point& newHead);
};
