// draw.cpp
#include <SDL2/SDL.h>
#include <cstdint>

extern "C" void draw_matrix(uint8_t* matrix) {
    const int cellSize = 40;
    const int gridSize = 10;
    const int windowSize = cellSize * gridSize;
    const int cellGap = 6; // Más espacio entre celdas
    const SDL_Color bgColor = {30, 30, 40, 255}; // Fondo más oscuro
    const SDL_Color gridColor = {60, 60, 80, 255}; // Cuadrícula más suave
    const SDL_Color cellColor = {0, 220, 120, 255}; // Verde más suave para las celdas vivas

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize, windowSize, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Fondo
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);

    // Dibujar celdas vivas
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            int idx = y * gridSize + x;
            if (matrix[idx]) {
                SDL_Rect rect = { x * cellSize + cellGap/2, y * cellSize + cellGap/2, cellSize - cellGap, cellSize - cellGap };
                SDL_SetRenderDrawColor(renderer, cellColor.r, cellColor.g, cellColor.b, cellColor.a);
                SDL_RenderFillRect(renderer, &rect);
                // Opcional: bordes suaves
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    // Dibujar cuadrícula
    SDL_SetRenderDrawColor(renderer, gridColor.r, gridColor.g, gridColor.b, gridColor.a);
    for (int i = 0; i <= gridSize; ++i) {
        SDL_RenderDrawLine(renderer, i * cellSize, 0, i * cellSize, windowSize);
        SDL_RenderDrawLine(renderer, 0, i * cellSize, windowSize, i * cellSize);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1000);  // Display for 1 second

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}