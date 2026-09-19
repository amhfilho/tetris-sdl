//
// Created by Filho, Antonio on 16/09/2026.
//
#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "Colors.h"

class Grid {
public:
    Grid(SDL_Renderer* renderer);
    void Initialize();
    void Draw();
    bool isCellOutside(int row, int col);
    bool isCellEmpty(int row, int col);
    bool isRowFull(int row);
    void ClearRow(int row);
    void MoveRowDown(int row, int completed);

    int ClearFullRows();
    int grid[20][10];
private:
    SDL_Renderer* renderer;
    int numRows;
    int numCols;
    int cellSize;
    std::vector<Color> colors;
};
