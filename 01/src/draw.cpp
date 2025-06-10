// draw.cpp
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdint>
#include <string>

extern "C" void draw_matrix(uint8_t* matrix) {
    const int cellSize = 40;
    const int gridSize = 10;
    const int matrixWidth = cellSize * gridSize;
    const int buttonWidth = 120;
    const int buttonHeight = 50;
    const int buttonMargin = 20;
    const int windowWidth = matrixWidth + buttonWidth + buttonMargin * 3;
    const int windowHeight = matrixWidth;
    const int cellGap = 6;
    
    // Colors
    const SDL_Color bgColor = {30, 30, 40, 255};
    const SDL_Color gridColor = {60, 60, 80, 255};
    const SDL_Color cellColor = {0, 220, 120, 255};
    const SDL_Color buttonColor = {70, 70, 90, 255};
    const SDL_Color buttonHoverColor = {90, 90, 110, 255};
    const SDL_Color textColor = {255, 255, 255, 255};

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    SDL_Window* window = SDL_CreateWindow("Game of Life", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        windowWidth, windowHeight, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    TTF_Font* font = TTF_OpenFont("./font/PressStart2P-Regular.ttf", 16);
    // Clear background
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);

    // Draw matrix cells
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            int idx = y * gridSize + x;
            if (matrix[idx]) {
                SDL_Rect rect = { 
                    x * cellSize + cellGap/2, 
                    y * cellSize + cellGap/2, 
                    cellSize - cellGap, 
                    cellSize - cellGap 
                };
                SDL_SetRenderDrawColor(renderer, cellColor.r, cellColor.g, cellColor.b, cellColor.a);
                SDL_RenderFillRect(renderer, &rect);
                
                // Soft borders
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    // Draw grid
    SDL_SetRenderDrawColor(renderer, gridColor.r, gridColor.g, gridColor.b, gridColor.a);
    for (int i = 0; i <= gridSize; ++i) {
        SDL_RenderDrawLine(renderer, i * cellSize, 0, i * cellSize, matrixWidth);
        SDL_RenderDrawLine(renderer, 0, i * cellSize, matrixWidth, i * cellSize);
    }

    // Calculate button positions
    int buttonX = matrixWidth + buttonMargin;
    int button1Y = windowHeight / 2 - buttonHeight - buttonMargin / 2;
    int button2Y = windowHeight / 2 + buttonMargin / 2;

    // Draw "Sig. Gen" button
    SDL_Rect button1 = {buttonX, button1Y, buttonWidth, buttonHeight};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &button1);
    SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
    SDL_RenderDrawRect(renderer, &button1);

    // Draw "Salir" button
    SDL_Rect button2 = {buttonX, button2Y, buttonWidth, buttonHeight};
    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &button2);
    SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
    SDL_RenderDrawRect(renderer, &button2);

    // Draw button text (if font loaded successfully)
    if (font) {
        // "Sig. Gen" text
        SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, "Next", textColor);
        if (textSurface1) {
            SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
            int textW1, textH1;
            SDL_QueryTexture(textTexture1, NULL, NULL, &textW1, &textH1);
            SDL_Rect textRect1 = {
                buttonX + (buttonWidth - textW1) / 2,
                button1Y + (buttonHeight - textH1) / 2,
                textW1, textH1
            };
            SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
            SDL_DestroyTexture(textTexture1);
            SDL_FreeSurface(textSurface1);
        }

        // "Salir" text
        SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, "Exit", textColor);
        if (textSurface2) {
            SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
            int textW2, textH2;
            SDL_QueryTexture(textTexture2, NULL, NULL, &textW2, &textH2);
            SDL_Rect textRect2 = {
                buttonX + (buttonWidth - textW2) / 2,
                button2Y + (buttonHeight - textH2) / 2,
                textW2, textH2
            };
            SDL_RenderCopy(renderer, textTexture2, NULL, &textRect2);
            SDL_DestroyTexture(textTexture2);
            SDL_FreeSurface(textSurface2);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1000);  // Display for 1 second

    // Cleanup
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
