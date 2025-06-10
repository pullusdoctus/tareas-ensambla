// draw.cpp
#include <SDL2/SDL.h>
#include <cstdint>

extern "C" void draw_matrix(uint8_t* matrix) {
    const int cellSize = 40;
    const int gridSize = 10;
    const int windowSize = cellSize * gridSize;
    const int cellGap = 4; // Espacio entre celdas

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize, windowSize, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Draw cells
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            int idx = y * gridSize + x;
            if (matrix[idx]) {
                SDL_Rect rect = { x * cellSize + cellGap/2, y * cellSize + cellGap/2, cellSize - cellGap, cellSize - cellGap };
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for live cells
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); // Gris para la cuadrícula
    for (int i = 0; i <= gridSize; ++i) {
        // Vertical
        SDL_RenderDrawLine(renderer, i * cellSize, 0, i * cellSize, windowSize);
        // Horizontal
        SDL_RenderDrawLine(renderer, 0, i * cellSize, windowSize, i * cellSize);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);  // Display for 1 second

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}