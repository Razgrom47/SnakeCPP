#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class UI {
public:
    UI(SDL_Renderer* renderer);
    ~UI();
    
    void renderGrid();
    void renderScore(int score, Uint32 playTime); 
    void renderGameOver(int score, Uint32 playTime);
    bool handleRestartButton(SDL_Event& e);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Rect restartButton;
    
    std::string formatTime(Uint32 milliseconds); 
    SDL_Texture* createTextTexture(const std::string& text, SDL_Color color);
};
