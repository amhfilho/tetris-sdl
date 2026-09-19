//
// Created by Filho, Antonio on 16/09/2026.
//
#include "Grid.h"
#include <SDL3/SDL.h>

#include "Block.h"

Grid::Grid(SDL_Renderer* renderer)
        : renderer(renderer)
        , numRows(20)
        , numCols(10)
        , cellSize(30) {
    Initialize();
    colors = GetCellColors();
}

void Grid::Initialize() {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            grid[i][j] = 0;
        }
    }
}

void Grid::Draw() {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            SDL_FRect block
            {
                static_cast<float>(cellSize) * col + 11,
                static_cast<float>(cellSize) * row + 11,
                static_cast<float>(cellSize-1),
                static_cast<float>(cellSize-1)
            };
            Color color = GetCellColors()[grid[row][col]];
            SDL_SetRenderDrawColor(renderer,color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &block);
        }
    }
}

bool Grid::isCellOutside(int row, int col) {
    return row < 0 || row >= numRows || col < 0 || col >= numCols;
}

bool Grid::isCellEmpty(int row, int col) {
    return !isCellOutside(row, col) && grid[row][col] == 0;
}

int Grid::ClearFullRows() {
    int completed = 0;
    for (int row = numRows - 1; row >= 0; row--) {
        if (isRowFull(row)) {
            ClearRow(row);
            completed++;
        }
        else if (completed > 0) {
            MoveRowDown(row, completed);
        }
    }
    return completed;
}

bool Grid::isRowFull(int row) {
    for (int col = 0; col < numCols; col++) {
        if (grid[row][col] == 0) {
            return false;
        }
    }
    return true;
}

void Grid::ClearRow(int row) {
    for (int col = 0; col < numCols; col++) {
        grid[row][col] = 0;
    }
}

void Grid::MoveRowDown(int row, int numRows) {
    for (int col = 0; col < numCols; col++) {
        grid[row + numRows][col] = grid[row][col];
        grid[row][col] = 0;
    }
}
