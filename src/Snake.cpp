#include "Snake.h"
#include "Constants.h"

void Snake::reset() { 
    body.clear();
    body.push_back({5, 5});
    direction = {1, 0};
    score = 0;
    shouldGrow = false;
}

void Snake::update(bool& gameOver) {
    SDL_Point newHead = {
        body.front().x + direction.x,
        body.front().y + direction.y
    };

    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH / GRID_SIZE ||
        newHead.y < 0 || newHead.y >= SCREEN_HEIGHT / GRID_SIZE ||
        checkSelfCollision(newHead)) {
        gameOver = true;
        return;
    }

    body.insert(body.begin(), newHead);
    
    if (!shouldGrow) {
        body.pop_back();
    }
    shouldGrow = false; // Reset growth flag
}

bool Snake::checkSelfCollision(const SDL_Point& newHead) {
    for (const auto& segment : body) {
        if (newHead.x == segment.x && newHead.y == segment.y)
            return true;
    }
    return false;
}

void Snake::changeDirection(SDL_Keycode key) {
    switch(key) {
        case SDLK_UP:    if (direction.y != 1)  direction = {0, -1}; break;
        case SDLK_DOWN:  if (direction.y != -1) direction = {0,  1}; break;
        case SDLK_LEFT:  if (direction.x != 1)  direction = {-1, 0}; break;
        case SDLK_RIGHT: if (direction.x != -1) direction = {1,  0}; break;
    }
}
