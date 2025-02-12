#include <iostream>
#include <iomanip> 
#include <sstream>
#include "UI.h"
#include "Constants.h"

// Constructor
UI::UI(SDL_Renderer* renderer) : renderer(renderer) {
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
    }
    font = TTF_OpenFont("assets/Shizuru-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
    }
    restartButton = { SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2 + 50, 150, 40 };
}

// Destructor
UI::~UI() {
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

void UI::renderGrid() {
    SDL_SetRenderDrawColor(renderer, GRID_COLOR.r, GRID_COLOR.g, GRID_COLOR.b, GRID_COLOR.a);
    for (int x = 0; x < SCREEN_WIDTH; x += GRID_SIZE) {
        SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
    }
    for (int y = 0; y < SCREEN_HEIGHT; y += GRID_SIZE) {
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
}

std::string UI::formatTime(Uint32 milliseconds) {
    int seconds = milliseconds / 1000;
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;
    
    std::stringstream ss;
    ss << hours << ":"
       << std::setw(2) << std::setfill('0') << minutes << ":"
       << std::setw(2) << std::setfill('0') << seconds;
    return ss.str();
}

SDL_Texture* UI::createTextTexture(const std::string& text, SDL_Color color) {
    if (!font) {
        std::cerr << "Font not loaded, can't create texture!" << std::endl;
        return nullptr;
    }
    
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Text surface creation failed: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!texture) {
        std::cerr << "Texture creation failed: " << SDL_GetError() << std::endl;
    }
    
    return texture;
}

void UI::renderScore(int score, Uint32 playTime) {
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Texture* scoreTexture = createTextTexture("Score: " + std::to_string(score), white);
    if (scoreTexture) {
        SDL_Rect scoreRect = { SCREEN_WIDTH - 150, 10, 140, 30 };
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
        SDL_DestroyTexture(scoreTexture);
    }
}

bool UI::handleRestartButton(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return (x >= restartButton.x && x <= restartButton.x + restartButton.w &&
                y >= restartButton.y && y <= restartButton.y + restartButton.h);
    }
    return false;
}

void UI::renderGameOver(int score, Uint32 playTime) {
    // Dark overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    // Game Over text
    SDL_Color red = { 255, 0, 0, 255 };
    SDL_Texture* gameOverTexture = createTextTexture("GAME OVER", red);
    if (gameOverTexture) {
        SDL_Rect gameOverRect = {
            SCREEN_WIDTH / 2 - 100, 
            SCREEN_HEIGHT / 2 - 100, 
            200, 
            50
        };
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
        SDL_DestroyTexture(gameOverTexture);
    }

    // Final Score
    SDL_Color white = { 255, 255, 255, 255 };
    std::string scoreText = "Final Score: " + std::to_string(score);
    SDL_Texture* scoreTexture = createTextTexture(scoreText, white);
    if (scoreTexture) {
        SDL_Rect scoreRect = {
            SCREEN_WIDTH / 2 - 100,
            SCREEN_HEIGHT / 2 - 30,
            200,
            30
        };
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
        SDL_DestroyTexture(scoreTexture);
    }

    // Time Played
    std::string timeText = "Time Played: " + formatTime(playTime);
    SDL_Texture* timeTexture = createTextTexture(timeText, white);
    if (timeTexture) {
        SDL_Rect timeRect = {
            SCREEN_WIDTH / 2 - 100,
            SCREEN_HEIGHT / 2 + 10,
            200,
            30
        };
        SDL_RenderCopy(renderer, timeTexture, nullptr, &timeRect);
        SDL_DestroyTexture(timeTexture);
    }

    // Restart button
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &restartButton);
    
    SDL_Texture* buttonText = createTextTexture("Restart", white);
    if (buttonText) {
        int texW = 0, texH = 0;
        SDL_QueryTexture(buttonText, nullptr, nullptr, &texW, &texH);
        SDL_Rect textRect = {
            restartButton.x + (restartButton.w - texW) / 2,
            restartButton.y + (restartButton.h - texH) / 2,
            texW,
            texH
        };
        SDL_RenderCopy(renderer, buttonText, nullptr, &textRect);
        SDL_DestroyTexture(buttonText);
    }
}
