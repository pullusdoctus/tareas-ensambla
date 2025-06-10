#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdint>
#include <iostream>

extern "C" {
    extern uint8_t matrix[100];
    void init_matrix();
    void update_matrix();
}

const int cellSize = 40;
const int gridSize = 10;
const int matrixWidth = cellSize * gridSize;
const int buttonWidth = 120;
const int buttonHeight = 50;
const int buttonMargin = 20;
const int windowWidth = matrixWidth + buttonWidth + buttonMargin * 3;
const int windowHeight = matrixWidth;
const int cellGap = 6;

void draw_matrix(SDL_Renderer* renderer, uint8_t* matrix, TTF_Font* font) {
    // Colors
    const SDL_Color bgColor = {30, 30, 40, 255};
    const SDL_Color gridColor = {60, 60, 80, 255};
    const SDL_Color cellColor = {0, 220, 120, 255};
    const SDL_Color buttonColor = {70, 70, 90, 255};
    const SDL_Color textColor = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);

    // Draw matrix cells
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            int idx = y * gridSize + x;
            if (matrix[idx]) {
                SDL_Rect rect = {
                    x * cellSize + cellGap / 2,
                    y * cellSize + cellGap / 2,
                    cellSize - cellGap,
                    cellSize - cellGap
                };
                SDL_SetRenderDrawColor(renderer, cellColor.r, cellColor.g, cellColor.b, cellColor.a);
                SDL_RenderFillRect(renderer, &rect);

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, gridColor.r, gridColor.g, gridColor.b, gridColor.a);
    for (int i = 0; i <= gridSize; ++i) {
        SDL_RenderDrawLine(renderer, i * cellSize, 0, i * cellSize, matrixWidth);
        SDL_RenderDrawLine(renderer, 0, i * cellSize, matrixWidth, i * cellSize);
    }

    // Draw buttons
    int buttonX = matrixWidth + buttonMargin;
    int button1Y = windowHeight / 2 - buttonHeight - buttonMargin / 2;
    int button2Y = windowHeight / 2 + buttonMargin / 2;

    SDL_Rect buttonNext = {buttonX, button1Y, buttonWidth, buttonHeight};
    SDL_Rect buttonExit = {buttonX, button2Y, buttonWidth, buttonHeight};

    SDL_SetRenderDrawColor(renderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(renderer, &buttonNext);
    SDL_RenderFillRect(renderer, &buttonExit);

    // Draw button text
    SDL_Surface* surfNext = TTF_RenderText_Blended(font, "Next", textColor);
    SDL_Surface* surfExit = TTF_RenderText_Blended(font, "Exit", textColor);

    SDL_Texture* texNext = SDL_CreateTextureFromSurface(renderer, surfNext);
    SDL_Texture* texExit = SDL_CreateTextureFromSurface(renderer, surfExit);

    int tw, th;
    SDL_QueryTexture(texNext, nullptr, nullptr, &tw, &th);
    SDL_Rect dstNext = {buttonX + (buttonWidth - tw)/2, button1Y + (buttonHeight - th)/2, tw, th};
    SDL_QueryTexture(texExit, nullptr, nullptr, &tw, &th);
    SDL_Rect dstExit = {buttonX + (buttonWidth - tw)/2, button2Y + (buttonHeight - th)/2, tw, th};

    SDL_RenderCopy(renderer, texNext, nullptr, &dstNext);
    SDL_RenderCopy(renderer, texExit, nullptr, &dstExit);

    SDL_DestroyTexture(texNext);
    SDL_DestroyTexture(texExit);
    SDL_FreeSurface(surfNext);
    SDL_FreeSurface(surfExit);

    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF Init failed: " << TTF_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Conway's Game of Life",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("./font/PressStart2P-Regular.ttf", 24);
    if (!font) {
        std::cerr << "Font loading failed: " << TTF_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    init_matrix();

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                int buttonX = matrixWidth + buttonMargin;
                int button1Y = windowHeight / 2 - buttonHeight - buttonMargin / 2;
                int button2Y = windowHeight / 2 + buttonMargin / 2;

                SDL_Rect buttonNext = {buttonX, button1Y, buttonWidth, buttonHeight};
                SDL_Rect buttonExit = {buttonX, button2Y, buttonWidth, buttonHeight};

                if (mx >= buttonNext.x && mx <= buttonNext.x + buttonNext.w &&
                    my >= buttonNext.y && my <= buttonNext.y + buttonNext.h) {
                    update_matrix();
                } else if (mx >= buttonExit.x && mx <= buttonExit.x + buttonExit.w &&
                           my >= buttonExit.y && my <= buttonExit.y + buttonExit.h) {
                    running = false;
                } else if (mx < matrixWidth && my < matrixWidth) {
                    int x = mx / cellSize;
                    int y = my / cellSize;
                    int idx = y * gridSize + x;
                    matrix[idx] = !matrix[idx];
                }
            }
        }
        draw_matrix(renderer, matrix, font);
        SDL_Delay(10);  // Small delay to reduce CPU usage
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
